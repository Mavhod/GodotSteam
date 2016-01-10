
#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

GodotSteam::GodotSteam()
{
	isInitSuccess = false;
}

bool GodotSteam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();
	return isInitSuccess;
}

bool GodotSteam::is_steam_running()
{
	return SteamAPI_IsSteamRunning();
}

int GodotSteam::get_appid()
{
	if ( SteamUtils() == NULL ) { return 0; }
	return SteamUtils()->GetAppID();
}

// Returns current app steam userdata path ( something like "C:\Progam Files\Steam\userdata\<SteamID>\<AppID>\local" )
String GodotSteam::get_userdata_path()
{
	if ( SteamUser() == NULL ) { return ""; }
	int cubBuffer = 256; // unsure if it represents char* size in SteamAPI
	char *pchBuffer = new char[cubBuffer];
	bool error = SteamUser()->GetUserDataFolder( pchBuffer, cubBuffer );
	// is this needed? (memory freeing)
	String path(pchBuffer);
	free(pchBuffer);
	// return String(pchBuffer);
	return path;
}


// Checks if Steam is running properly (got connection to Steam servers)
bool GodotSteam::user_is_logged()
{
	if ( SteamUser() == NULL ) { return false; }
	return SteamUser()->BLoggedOn();
}

// --- Currently unused - using get_account_id() instead
// Returns Steam3 ID of current user
int GodotSteam::user_get_id()
{
	if ( SteamUser() == NULL ) { return 0; }
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.ConvertToUint64();
}

// Returns current user nickname
String GodotSteam::user_get_name()
{
	if ( SteamFriends() == NULL ) { return "NOT_INITIALIZED"; }
	return SteamFriends()->GetPersonaName();
}

// Get user Steam level
int GodotSteam::user_get_steam_level()
{
	if ( SteamUser() == NULL ) { return 0; }
	return SteamUser()->GetPlayerSteamLevel();
}

// Get user Account ID, usually it's the same as user Steam ID
int GodotSteam::user_get_account_id()
{
	if ( SteamUser() == NULL ) { return 0; }
	return int(SteamUser()->GetSteamID().GetAccountID());
}

// Get user universe (check constants). Mostly will return 1 - UNIVERSE_PUBLIC
int GodotSteam::user_get_universe()
{
	if ( SteamUser() == NULL ) { return 0; }
	return int(SteamUser()->GetSteamID().GetEUniverse());
}

// Get link to user's SteamCommunity profile
String GodotSteam::user_get_profile_link()
{
	if ( SteamUser() == NULL ) { return ""; }
	String link="http://steamcommunity.com/profiles/";
	String u_account_id = itos(user_get_account_id());
	String u_universe = itos(user_get_universe());
	// [ <AccountType> : <Universe> : <AccountID> ]
	// U - individual account
	link += "[U:"+u_universe+":"+u_account_id+"]";
	return link;
}

// Set data to be replicated to friends so that they can join your game
void GodotSteam::user_set_server_info(int host_id, const String& server_ip, int port)
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
	SteamUser()->AdvertiseGame(CSteamID(uint64(host_id)), addr.host, port);
}

// --- Disabled: requires callbacks handling - without them might locally display incorrect nickname (Steam servers might not accept or receive new nickname)
// Sets user nickname
//void GodotSteam::set_username(const String& new_name)
//{
//	if (SteamFriends() != NULL)
//		SteamFriends()->SetPersonaName( new_name.utf8().get_data() );
//}


// Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
// start & hook the game process, so this function will initially return false while the overlay is loading.
bool GodotSteam::overlay_is_enabled()
{
	if ( SteamUtils() == NULL ) { return false; }
	return SteamUtils()->IsOverlayEnabled();
}

// Change position(corner) at which notifications (messages, etc.) will be displayed
// TOP_LEFT=0 | TOP_RIGHT=1 | BOT_LEFT=2 | BOT_RIGHT=3
void GodotSteam::overlay_set_notify_pos( int pos )
{
	if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) { return; }
	SteamUtils()->SetOverlayNotificationPosition( ENotificationPosition(pos) ) ;
}

// Open URL using Steam overlay browser
void GodotSteam::overlay_open_url(const String& url)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToWebPage( url.utf8().get_data() );
}


void GodotSteam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&GodotSteam::init);
	ObjectTypeDB::bind_method("is_steam_running",&GodotSteam::is_steam_running);
	ObjectTypeDB::bind_method("get_appid",&GodotSteam::get_appid);
	ObjectTypeDB::bind_method("get_userdata_path",&GodotSteam::get_userdata_path);
	
	ObjectTypeDB::bind_method("user_is_logged",&GodotSteam::user_is_logged);
	ObjectTypeDB::bind_method("user_get_id",&GodotSteam::user_get_account_id); // return account id instead of steam id converted to uint64
//	ObjectTypeDB::bind_method("user_get_account_id",&GodotSteam::user_get_account_id);
//	ObjectTypeDB::bind_method("user_get_universe",&GodotSteam::user_get_universe); // probably no one will ever need it so disabling it in GDScript
	ObjectTypeDB::bind_method("user_get_name",&GodotSteam::user_get_name);
	ObjectTypeDB::bind_method("user_get_steam_level",&GodotSteam::user_get_steam_level);
	ObjectTypeDB::bind_method("user_get_profile_link",&GodotSteam::user_get_profile_link);
	ObjectTypeDB::bind_method(_MD("user_set_server_info","server_SteamID","server_ip","port"),&GodotSteam::user_set_server_info);
//	ObjectTypeDB::bind_method("set_username",&GodotSteam::set_username);
	
	ObjectTypeDB::bind_method("overlay_is_enabled",&GodotSteam::overlay_is_enabled);
	ObjectTypeDB::bind_method(_MD("overlay_set_notification_pos","0-3"),&GodotSteam::overlay_set_notify_pos);
	ObjectTypeDB::bind_method(_MD("overlay_open_url","url"),&GodotSteam::overlay_open_url);
	
	BIND_CONSTANT(TOP_LEFT);
	BIND_CONSTANT(TOP_RIGHT);
	BIND_CONSTANT(BOT_LEFT);
	BIND_CONSTANT(BOT_RIGHT);
	
// Disabling since user_get_universe is also disabled
//	BIND_CONSTANT(UNIVERSE_INVALID);
//	BIND_CONSTANT(UNIVERSE_PUBLIC);
//	BIND_CONSTANT(UNIVERSE_BETA);
//	BIND_CONSTANT(UNIVERSE_INTERNAL);
//	BIND_CONSTANT(UNIVERSE_DEV);
}

GodotSteam::~GodotSteam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
}
