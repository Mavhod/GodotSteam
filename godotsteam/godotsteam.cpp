
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

// Checks if Steam is running properly (got connection to Steam servers)
bool GodotSteam::user_is_logged()
{
	if ( SteamUser() == NULL ) { return false; }
	return SteamUser()->BLoggedOn();
}

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

// Set data to be replicated to friends so that they can join your game
void GodotSteam::set_server_info(int host_id, const String& server_ip, int port)
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

// Disabled: requires callbacks handling - without them might locally display incorrect nickname (Steam servers might not accept new nickname)
// Sets user nickname
//void GodotSteam::set_username(const String& new_name)
//{
//	if (SteamFriends() != NULL)
//		SteamFriends()->SetPersonaName( new_name.utf8().get_data() );
//}



void GodotSteam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&GodotSteam::init);
	ObjectTypeDB::bind_method("is_steam_running",&GodotSteam::is_steam_running);
	
	ObjectTypeDB::bind_method("user_is_logged",&GodotSteam::user_is_logged);
	ObjectTypeDB::bind_method("user_get_id",&GodotSteam::user_get_id);
	ObjectTypeDB::bind_method("user_get_name",&GodotSteam::user_get_name);
	ObjectTypeDB::bind_method("user_get_steam_level",&GodotSteam::user_get_steam_level);
	
	ObjectTypeDB::bind_method("set_server_info",&GodotSteam::set_server_info);
//	ObjectTypeDB::bind_method("set_username",&GodotSteam::set_username);
}

GodotSteam::~GodotSteam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
}
