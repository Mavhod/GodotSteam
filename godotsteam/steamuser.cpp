
#include "steamuser.h"

#include "core/io/ip_address.h"
#include "core/io/ip.h"


GSteamUser::GSteamUser()
{
	isEmpty = true;
}

GSteamUser::GSteamUser( ISteamUser* iSteamUser )
{
	this->iSteamUser = iSteamUser;
	isEmpty = false;
}


// Returns Steam3 ID of current user
int GSteamUser::get_id()
{
	if (isEmpty) { return 0; }
	CSteamID cSteamID = iSteamUser->GetSteamID();
	return cSteamID.ConvertToUint64();
//	return 0;
}

// Checks if Steam is running properly (got connection to Steam servers)
bool GSteamUser::is_logged()
{
	if (isEmpty) { return false; }
	return iSteamUser->BLoggedOn();
//	return false;
}

// Does it have ISteamUser assigned?
bool GSteamUser::is_empty() { return isEmpty; }

// Get user Steam level
int GSteamUser::get_steam_level() { return iSteamUser->GetPlayerSteamLevel(); }

// Set data to be replicated to friends so that they can join your game
void GSteamUser::advertise_game(int host_id, const String& server_ip, int port)
{
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
	iSteamUser->AdvertiseGame(host_id, addr.host, port);
}



// Sets ISteamUser (check steam/isteamuser.h)
void GSteamUser::set_isteamuser( ISteamUser* iSteamUser )
{
	this->iSteamUser = iSteamUser;
	isEmpty = false;
}

// Gets assigned ISteamUser
ISteamUser* GSteamUser::get_isteamuser() { return iSteamUser; }

void GSteamUser::_bind_methods()
{
	ObjectTypeDB::bind_method("get_id",&GSteamUser::get_id);
	ObjectTypeDB::bind_method("is_logged",&GSteamUser::is_logged);
	ObjectTypeDB::bind_method("is_empty",&GSteamUser::is_empty);
	ObjectTypeDB::bind_method("get_steam_level",&GSteamUser::get_steam_level);
	ObjectTypeDB::bind_method("advertise_game",&GSteamUser::advertise_game);
}

GSteamUser::~GSteamUser()
{
	//free(iSteamUser);
}