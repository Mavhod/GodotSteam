
#include "gsteamuser.h"


_SteamUser::_SteamUser(CSteamID cSID)
{
	setCSteamID( cSID );
	// _steamid_changed() => updateType() = try to find its type
}

bool _SteamUser::updateType()
{
	int old_type = get_user_type();
	setUserType(INVALID);
	CSteamID your_id = getCSteamID();
	if ( SteamUser() != NULL && your_id == SteamUser()->GetSteamID() )
	{ 
		setUserType(YOU);
	}
	else if ( SteamFriends() != NULL )
	{
		if ( SteamFriends()->HasFriend(your_id,0xFFFF) == true ) // 0x04 - "regular" friend, 0xFFFF - any (mostly friends+requests+blocked...)
		{ setUserType(USER); }
	}
	return (old_type != get_user_type());
}


// sets to INVALID if unknown type
void _SteamUser::setUserType(int uType)
{
	if ( uType<0 || uType>USER_TYPE_MAX )
		uType = INVALID;
	user_type = uType;
}

String _SteamUser::get_name()
{
	if ( SteamFriends() == NULL ) { return ""; }
	if ( get_user_type() == YOU )
		return SteamFriends()->GetPersonaName();
	else if ( get_user_type() == USER )
		return SteamFriends()->GetFriendPersonaName( getCSteamID() );
	return "";
}

int _SteamUser::get_state()
{
	if ( SteamFriends() == NULL ) { return 0; }
	if ( get_user_type() == YOU )
		return SteamFriends()->GetPersonaState();
	else if ( get_user_type() == USER )
		return SteamFriends()->GetFriendPersonaState( getCSteamID() );
	return 0;
}

int _SteamUser::get_steamlevel()
{
	if ( SteamUser() == NULL ) { return -1; }
	if ( get_user_type() == YOU )
	{ return SteamUser()->GetPlayerSteamLevel(); }
	else if ( get_user_type() == USER )
	{ return SteamFriends()->GetFriendSteamLevel( getCSteamID() ); }
	return -1;
}

// moved to 'Steam'
bool _SteamUser::is_logged()
{
	if ( SteamUser() == NULL ) { return false; }
	if ( get_user_type() == YOU )
	{
		return SteamUser()->BLoggedOn();
	}
	return false;
}
// ---

String _SteamUser::get_game_info(const String& s_key)
{
	if ( SteamFriends() == NULL ) { return ""; }
	if ( get_user_type() == YOU || get_user_type() == USER )
	{
		return SteamFriends()->GetFriendRichPresence(getCSteamID(), s_key.utf8().get_data());
	}
	return "";
}

// Checks if user is on your friends list
bool _SteamUser::is_friend()
{
	if ( SteamFriends() == NULL ) { return false; }
	if ( get_user_type() == USER )
	{
		return SteamFriends()->HasFriend(getCSteamID(),0x04);
	}
	return false;	
}

int _SteamUser::get_relationship()
{
	if ( SteamFriends() == NULL ) { return REL_NONE; }
	if ( get_user_type() == USER )
	{
		return SteamFriends()->GetFriendRelationship( getCSteamID() );
	}
	return REL_NONE;
}

void _SteamUser::request_game_info()
{
	if ( SteamFriends() == NULL || get_user_type() != USER ) { return; }
	SteamFriends()->RequestFriendRichPresence( getCSteamID() );
}

void _SteamUser::_game_info_received( FriendRichPresenceUpdate_t* rich_update )
{
	if ( rich_update->m_steamIDFriend == getCSteamID() )
		emit_signal("rich_presence_rec");
}

Image _SteamUser::draw_avatar(int iSize, uint8* iBuffer) 
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

void _SteamUser::_avatar_loaded( AvatarImageLoaded_t* avatarData )
{
	if (avatarData->m_steamID != getCSteamID() ) { return; }
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

bool _SteamUser::load_avatar(int size)
{
	if (size<AVATAR_SMALL || size > AVATAR_LARGE) { return false; }
	if ( SteamFriends() == NULL ) { return false; }
	int iHandle=-2;
	switch(size)
	{
		case AVATAR_SMALL:
		{
			iHandle = SteamFriends()->GetSmallFriendAvatar( getCSteamID() );
			size = 32;
			break;
		}
		case AVATAR_MEDIUM:
		{
			iHandle = SteamFriends()->GetMediumFriendAvatar( getCSteamID() );
			size = 64;
			break;
		}
		case AVATAR_LARGE:
		{
			iHandle = SteamFriends()->GetLargeFriendAvatar( getCSteamID() );
			size = 184;
			break;
		}
		default:
			return false;
	}
	if ( iHandle <= 0 )
	{
		if ( iHandle == -1)
		{
			// inform that this one is still loading
			return true;
		}
		// invalid handle
		return false;
	}
	// already loaded

	// simulate callback
	AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = getCSteamID();
	avatarData->m_iImage = iHandle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	_avatar_loaded(avatarData);
	delete avatarData;
	return true;
}

void _SteamUser::set_played_with()
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->SetPlayedWith( getCSteamID() );
}



void _SteamUser::_bind_methods()
{
	ObjectTypeDB::bind_method("get_name",&_SteamUser::get_name);
	ObjectTypeDB::bind_method("get_status",&_SteamUser::get_state);
	ObjectTypeDB::bind_method(_MD("load_avatar","size"),&_SteamUser::load_avatar,DEFVAL(AVATAR_MEDIUM));
	ObjectTypeDB::bind_method("get_user_type",&_SteamUser::get_user_type);
	ObjectTypeDB::bind_method("get_steamlevel",&_SteamUser::get_steamlevel);
	ObjectTypeDB::bind_method("set_played_with",&_SteamUser::set_played_with);
	ObjectTypeDB::bind_method(_MD("get_game_info","key"),&_SteamUser::get_game_info);
	ObjectTypeDB::bind_method(_MD("request_game_info"),&_SteamUser::request_game_info);
	ObjectTypeDB::bind_method("is_friend",&_SteamUser::is_friend);
	// ObjectTypeDB::bind_method("is_logged",&_SteamUser::is_logged);
	// ObjectTypeDB::bind_method(_MD("get_relationship"),&_SteamUser::get_relationship);
	
	ADD_SIGNAL(MethodInfo("rpresence_updated"));
	ADD_SIGNAL(MethodInfo("avatar_loaded",PropertyInfo(Variant::INT,"size"),PropertyInfo(Variant::IMAGE,"avatar")));
	
	BIND_CONSTANT(INVALID);
	BIND_CONSTANT(YOU);
	BIND_CONSTANT(USER);
	
	BIND_CONSTANT(OFFLINE);
	BIND_CONSTANT(ONLINE);
	BIND_CONSTANT(BUSY);
	BIND_CONSTANT(AWAY);
	BIND_CONSTANT(SNOOZE);
	BIND_CONSTANT(LF_TRADE);
	BIND_CONSTANT(LF_PLAY);
	
	BIND_CONSTANT(AVATAR_SMALL);
	BIND_CONSTANT(AVATAR_MEDIUM);
	BIND_CONSTANT(AVATAR_LARGE);
}

_SteamUser::~_SteamUser()
{
	
}




