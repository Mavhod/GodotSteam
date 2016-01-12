
#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

Steam* Steam::singleton = NULL;

Steam::Steam()
{
	isInitSuccess = false;
	singleton = this;
}

Steam* Steam::get_singleton() { return singleton; }

bool Steam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();
	return isInitSuccess;
}

bool Steam::is_steam_running()
{
	return SteamAPI_IsSteamRunning();
}

int Steam::get_appid()
{
	if ( SteamUtils() == NULL ) { return 0; }
	return SteamUtils()->GetAppID();
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




// --- Disabled: requires callbacks handling - without them might locally display incorrect nickname (Steam servers might not accept or receive new nickname)
// Sets user nickname
//void Steam::set_username(const String& new_name)
//{
//	if (SteamFriends() != NULL)
//		SteamFriends()->SetPersonaName( new_name.utf8().get_data() );
//}

// Set data to be replicated to friends so that they can join your game
void Steam::user_set_game_info(Ref<SteamID> gameserver, const String& server_ip, int port)
{
	if ( SteamUser() == NULL ) { return; }
	// resolve address and convert it to int (IP_Address) union
	IP_Address addr(IP::get_singleton()->resolve_hostname(server_ip));
	// for some reason bytes are in wrong order, need to swap them
	for(int i=0;i<2;i++)
	{
		uint8 temp = addr.field[i];
		addr.field[i] = addr.field[3-i];
		addr.field[3-i] = temp;
	}
//	printf("IP int: %ld",addr.host);
	SteamUser()->AdvertiseGame(gameserver->getCSteamID(), addr.host, port); 
}



// Returns friends array filtered by given filtered
Array Steam::friends_getall( int filter ) // default = NOT_OFFLINE
{
	// If can't access SteamAPI then last generated list will be returned (stored in `friendList`)
	updateFriendList(filter);
	return friendList;
}
// this one probably will be called by some callbacks
void Steam::updateFriendList(int filter)
{
	if ( SteamFriends() == NULL ) { return; }
	// default filter = lastFriendsFilter
	if ( filter == -1 ) { filter = lastFriendsFilter; }
	// if filter out of range then filter = ALL
	if ( filter<0 || filter>ALL ) { filter = ALL; }
	friendList.clear();
	int list_size = SteamFriends()->GetFriendCount(0x04); // "regular" friend 0x04 (see sdk EFriendFlags)
	for(int i=0;i<list_size;i++)
	{
		CSteamID f_steam_id = SteamFriends()->GetFriendByIndex( i, 0x04 ); // "regular" friend 0x04 (see EFriendFlags)
		int f_state = int( SteamFriends()->GetFriendPersonaState(f_steam_id) );
		if ( !(filter == ALL || filter == f_state || (filter==NOT_OFFLINE && f_state!=OFFLINE) ) ) 
			{ continue; }
		Ref<_SteamUser> new_friend = memnew( _SteamUser(f_steam_id) ); // ,_SteamUser::FRIEND
		friendList.push_back(new_friend);
	}
	lastFriendsFilter = filter;
}

Array Steam::groups_getall() // default = NOT_OFFLINE
{
	// If can't access SteamAPI then last generated list will be returned (stored in `groupsList`)
	updateGroupList();
	return groupList;
}
// this one probably will be called by some callbacks
void Steam::updateGroupList()
{
	if ( SteamFriends() == NULL ) { return; }
	groupList.clear();
	int list_size = SteamFriends()->GetClanCount();
	for(int i=0;i<list_size;i++)
	{
		CSteamID g_steam_id = SteamFriends()->GetClanByIndex(i);
		Ref<_SteamGroup> next_group = memnew( _SteamGroup(g_steam_id) );
		groupList.push_back(next_group);
	}
}


// Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
// start & hook the game process, so this function will initially return false while the overlay is loading.
bool Steam::overlay_is_enabled()
{
	if ( SteamUtils() == NULL ) { return false; }
	return SteamUtils()->IsOverlayEnabled();
}

// Change position(corner) at which notifications (messages, etc.) will be displayed
// TOP_LEFT=0 | TOP_RIGHT=1 | BOT_LEFT=2 | BOT_RIGHT=3
void Steam::overlay_set_notify_pos( int pos )
{
	if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) { return; }
	SteamUtils()->SetOverlayNotificationPosition( ENotificationPosition(pos) ) ;
}

// activates the game overlay, with an optional dialog to open 
// valid options are "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements"
void Steam::overlay_open(const String& url)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlay( url.utf8().get_data() );
}

// activates game overlay to a specific place
// valid options are
//		"steamid" - opens the overlay web browser to the specified user or groups profile
//		"chat" - opens a chat window to the specified user, or joins the group chat 
//		"jointrade" - opens a window to a Steam Trading session that was started with the ISteamEconomy/StartTrade Web API
//		"stats" - opens the overlay web browser to the specified user's stats
//		"achievements" - opens the overlay web browser to the specified user's achievements
//		"friendadd" - opens the overlay in minimal mode prompting the user to add the target user as a friend
//		"friendremove" - opens the overlay in minimal mode prompting the user to remove the target friend
//		"friendrequestaccept" - opens the overlay in minimal mode prompting the user to accept an incoming friend invite
//		"friendrequestignore" - opens the overlay in minimal mode prompting the user to ignore an incoming friend invite
void Steam::overlay_open_user(const String& url, Ref<SteamID> user)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToUser( url.utf8().get_data(), user->getCSteamID() );
	//SteamFriends()->ActivateGameOverlayToUser( url.utf8().get_data(), CSteamID(uint64(steam_id)) );
}

// Open URL using Steam overlay browser
void Steam::overlay_open_url(const String& url)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToWebPage( url.utf8().get_data() );
}

// Opens Steam App Store
void Steam::overlay_open_store(int app_id)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToStore( AppId_t(app_id), EOverlayToStoreFlag(0) );
}


Ref<_SteamUser> Steam::get_user()
{
	if ( SteamFriends() != NULL && yourUser == NULL )
	{
		CSteamID cSteamID = SteamUser()->GetSteamID();
		yourUser = Ref<_SteamUser>( memnew( _SteamUser(cSteamID) ) ); // ,_SteamUser::YOU
	}
	return yourUser;
}

void Steam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&Steam::init);
	ObjectTypeDB::bind_method("is_steam_running",&Steam::is_steam_running);
	ObjectTypeDB::bind_method("get_user",&Steam::get_user);
	ObjectTypeDB::bind_method("get_appid",&Steam::get_appid);
	ObjectTypeDB::bind_method("get_userdata_path",&Steam::get_userdata_path);
	
	ObjectTypeDB::bind_method(_MD("set_game_info","SteamGameServer","server_ip","port"),&Steam::user_set_game_info);

	ObjectTypeDB::bind_method(_MD("get_friends","filter"),&Steam::friends_getall,DEFVAL(NOT_OFFLINE));
	ObjectTypeDB::bind_method("get_groups",&Steam::groups_getall);
	ObjectTypeDB::bind_method("overlay_is_enabled",&Steam::overlay_is_enabled);
	ObjectTypeDB::bind_method(_MD("overlay_set_notification_pos","0-3"),&Steam::overlay_set_notify_pos);
	ObjectTypeDB::bind_method(_MD("overlay_open","type"),&Steam::overlay_open,DEFVAL(""));
	ObjectTypeDB::bind_method(_MD("overlay_open_user","type","SteamUser"),&Steam::overlay_open_user);
	ObjectTypeDB::bind_method(_MD("overlay_open_url","url"),&Steam::overlay_open_url);
	ObjectTypeDB::bind_method(_MD("overlay_open_store","appID"),&Steam::overlay_open_store,DEFVAL(0));
	
	BIND_CONSTANT(TOP_LEFT);
	BIND_CONSTANT(TOP_RIGHT);
	BIND_CONSTANT(BOT_LEFT);
	BIND_CONSTANT(BOT_RIGHT);
	
	BIND_CONSTANT(OFFLINE); // 0
	BIND_CONSTANT(ONLINE); // 1
	BIND_CONSTANT(BUSY); // ...
	BIND_CONSTANT(AWAY);
	BIND_CONSTANT(SNOOZE);
	BIND_CONSTANT(NOT_OFFLINE); // custom - 7
	BIND_CONSTANT(ALL); // custom - 8


//	BIND_CONSTANT(UNIVERSE_INVALID);
//	BIND_CONSTANT(UNIVERSE_PUBLIC);
//	BIND_CONSTANT(UNIVERSE_BETA);
//	BIND_CONSTANT(UNIVERSE_INTERNAL);
//	BIND_CONSTANT(UNIVERSE_DEV);
}

Steam::~Steam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}
