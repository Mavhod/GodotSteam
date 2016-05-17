
#include "godotsteam.h"


Steam* Steam::singleton = NULL;

Steam::Steam()
{
	isInitSuccess = false;
	singleton = this;
}

Steam* Steam::get_singleton() { return singleton; }


CSteamID Steam::create_steamid( uint32 steamId, int accountType )
{
	CSteamID cSteamID;
	if (accountType < 0 || accountType >= k_EAccountTypeMax )
		accountType = 1; // k_EAccountTypeIndividual
	cSteamID.Set( steamId, EUniverse(k_EUniversePublic), EAccountType(accountType) );
	return cSteamID;
}



int Steam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();
	int err = FAILED;
	if (isInitSuccess)
		{ err = OK; }
	
	if ( !SteamAPI_IsSteamRunning() )
		{ err = ERR_NO_CLIENT; }
	else if ( !SteamUser()->BLoggedOn() )
		{ err = ERR_NO_CONNECTION; }
	
	if ( err == OK && SteamUserStats() != NULL )
	{
		// load stats and achievements
		SteamUserStats()->RequestCurrentStats();
	}
	
	return err;
}




// Returns current app steam userdata path ( something like "C:\Progam Files\Steam\userdata\<SteamID>\<AppID>\local" )
String Steam::get_userdata_path()
{
	if ( SteamUser() == NULL ) { return ""; }
	const int cubBuffer = 256; // unsure if it represents char* size in SteamAPI
	const char *pchBuffer = new const char[cubBuffer];
	bool error = SteamUser()->GetUserDataFolder( (char*)pchBuffer, cubBuffer );
	// ?error? handling?
	return pchBuffer;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		USER
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// returns name of current user, or user with given steamId
String Steam::get_username(int steamId)
{
	if ( SteamUser() != NULL && (steamId < 0 || steamId == SteamUser()->GetSteamID().GetAccountID()) )
	{
		return SteamFriends()->GetPersonaName();
	}
	else if ( SteamFriends() != NULL && steamId > 0 )
	{
		// CSteamID( unAccountID, eUniverse, eAccountType )
		// CSteamID( (uint32)steamId, EUniverse(k_EUniversePublic), EAccountType(k_EAccountTypeIndividual) );
		CSteamID friendID = create_steamid( steamId );
		bool isDataLoading = SteamFriends()->RequestUserInformation( friendID, true ); // nameOnly = true
		if (!isDataLoading) // data already loaded
		{
			return SteamFriends()->GetFriendPersonaName( friendID );
		}
		// else - requested data and waiting for callback
		// we could wait for callback here, but what if we're not processing them? app freeze?
	}
	return "";
}

// returns SteamID 3 of current user
int Steam::get_steamid()
{
	if ( SteamUser() != NULL )
	{
		return SteamUser()->GetSteamID().GetAccountID();
	}
	return 0;
}

// loads avatar from your Steam profile
void Steam::load_avatar(int size)
{
	if (size<AVATAR_SMALL || size > AVATAR_LARGE) { return; }
	if ( SteamFriends() == NULL ) { return; }
	
	int iHandle=-2;
	switch(size)
	{
		case AVATAR_SMALL:
		{
			iHandle = SteamFriends()->GetSmallFriendAvatar( SteamUser()->GetSteamID() );
			size = 32; break;
		}
		case AVATAR_MEDIUM:
		{
			iHandle = SteamFriends()->GetMediumFriendAvatar( SteamUser()->GetSteamID() );
			size = 64; break;
		}
		case AVATAR_LARGE:
		{
			iHandle = SteamFriends()->GetLargeFriendAvatar( SteamUser()->GetSteamID() );
			size = 184; break;
		}
		default:
			return;
	}
	if ( iHandle <= 0 )
	{
		if ( iHandle == -1)
		{
			// this one is still loading
			return;
		}
		// invalid handle
		return;
	}
	// already loaded

	// simulate callback
	AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = SteamUser()->GetSteamID();
	avatarData->m_iImage = iHandle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	_avatar_loaded(avatarData);
	delete avatarData;
	return;
}
// emits avatar_loaded signal
		void Steam::_avatar_loaded( AvatarImageLoaded_t* avatarData )
		{
			if (avatarData->m_steamID != SteamUser()->GetSteamID() ) { return; }
			int size = avatarData->m_iWide;
			// get img buffer
			int buffSize = 4*size*size;
			uint8* iBuffer = new uint8[buffSize];
			bool success = SteamUtils()->GetImageRGBA(avatarData->m_iImage,iBuffer,buffSize);
			if (!success)
			{
				printf("[Steam]Failed to load image buffer from callback\n");
				return;
			}
			int rSize;
			if (size == 32)
				{ rSize = AVATAR_SMALL; }
			else if (size == 64)
				{ rSize = AVATAR_MEDIUM;}
			else if (size == 184)
				{ rSize = AVATAR_LARGE; }
			else
			{
				printf("[Steam]Invalid avatar size from callback\n");
				return;
			}
			Image avatar = draw_avatar(size,iBuffer);
			call_deferred("emit_signal","avatar_loaded",rSize,avatar);
		}
// creates square image from RGBA buffer 
		Image Steam::draw_avatar(int iSize, uint8* iBuffer) 
		{
			// Apply buffer to Image
			Image avatar(iSize,iSize,false,Image::FORMAT_RGBA);
			for(int y=0;y<iSize;y++)
			{
				for(int x=0;x<iSize;x++)
				{
					int i = 4*(x+y*iSize);
					float r = float(iBuffer[i])/255;
					float g = float(iBuffer[i+1])/255;
					float b = float(iBuffer[i+2])/255;
					float a = float(iBuffer[i+3])/255;
					avatar.put_pixel(x,y, Color(r,g,b,a) );
				}
			}
			return avatar;
		}
//




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		STATS & ACHIEVEMENTS
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Steam::unlock_achiv(const String& aName)
{
	if ( SteamUserStats() == NULL ) { return; }
	SteamUserStats()->SetAchievement( aName.utf8().get_data() );
}
bool Steam::has_achiv(const String& aName)
{
	if ( SteamUserStats() == NULL ) { return false; }
	bool achieved;
	SteamUserStats()->GetAchievement( aName.utf8().get_data(), &achieved );
	return achieved;
}
void Steam::reset_achiv(const String& aName)
{
	if ( SteamUserStats() == NULL ) { return; }
	SteamUserStats()->ClearAchievement( aName.utf8().get_data() );
}
// Stats
// Int
void Steam::set_stat_i(const String& sName, int sVal)
{
	if ( SteamUserStats() == NULL ) { return; }
	SteamUserStats()->SetStat( sName.utf8().get_data(), (int32)sVal );
}
int Steam::get_stat_i(const String& sName)
{
	if ( SteamUserStats() == NULL ) { return -2; }
	int32 sVal=0;
	SteamUserStats()->GetStat( sName.utf8().get_data(), &sVal );
	return sVal;
}
// Float
void Steam::set_stat_f(const String& sName, float sVal)
{
	if ( SteamUserStats() == NULL ) { return; }
	SteamUserStats()->SetStat( sName.utf8().get_data(), sVal );
}
float Steam::get_stat_f(const String& sName)
{
	if ( SteamUserStats() == NULL ) { return -2; }
	float sVal=0;
	SteamUserStats()->GetStat( sName.utf8().get_data(), &sVal );
	return sVal;
}

// Sends your stats & achievements state then requests current state from it
void Steam::sync_stats()
{
	if ( SteamUserStats() == NULL ) { return; }
	SteamUserStats()->StoreStats();
	SteamUserStats()->RequestCurrentStats();
}







void Steam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&Steam::init);
	ObjectTypeDB::bind_method("run_callbacks",&Steam::run_callbacks);
	ObjectTypeDB::bind_method("get_userdata_path",&Steam::get_userdata_path);
	// user
	ObjectTypeDB::bind_method(_MD("get_username","steamID"),&Steam::get_username,DEFVAL(0));
	ObjectTypeDB::bind_method("get_steamid",&Steam::get_steamid);
	// stats & achievements
	ObjectTypeDB::bind_method(_MD("unlock_achiv","api_name"),&Steam::unlock_achiv);
	ObjectTypeDB::bind_method(_MD("has_achiv","api_name"),&Steam::has_achiv);
	ObjectTypeDB::bind_method(_MD("set_stat_i","api_name","value"),&Steam::set_stat_i);
	ObjectTypeDB::bind_method(_MD("get_stat_i","api_name"),&Steam::get_stat_i);
	ObjectTypeDB::bind_method(_MD("set_stat_f","api_name","value"),&Steam::set_stat_f);
	ObjectTypeDB::bind_method(_MD("get_stat_f","api_name"),&Steam::get_stat_f);
	ObjectTypeDB::bind_method("sync_stats",&Steam::sync_stats);
	// other
	ObjectTypeDB::bind_method(_MD("load_avatar","size"),&Steam::load_avatar,DEFVAL(AVATAR_MEDIUM));
	
	
	ADD_SIGNAL(MethodInfo("avatar_loaded",PropertyInfo(Variant::INT,"size"),PropertyInfo(Variant::IMAGE,"avatar")));
	
	// init errors
	BIND_CONSTANT(ERR_NO_CLIENT);
	BIND_CONSTANT(ERR_NO_CONNECTION);
	// avatar sizes
	BIND_CONSTANT(AVATAR_SMALL);
	BIND_CONSTANT(AVATAR_MEDIUM);
	BIND_CONSTANT(AVATAR_LARGE);
}

Steam::~Steam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}
