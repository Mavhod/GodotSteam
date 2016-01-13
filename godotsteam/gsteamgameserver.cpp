
#include "gsteamgameserver.h"
#include <steam/steam_gameserver.h>

_SteamGameServer* _SteamGameServer::singleton = NULL;

_SteamGameServer::_SteamGameServer()
{
	singleton = this;
}

_SteamGameServer* _SteamGameServer::get_singleton() { return singleton; }

void _SteamGameServer::_bind_methods()
{
	// ObjectTypeDB::bind_method("get_name",&_SteamGameServer::get_name);
	// BIND_CONSTANT(INVALID);
}

_SteamGameServer::~_SteamGameServer()
{
	if(isInitSuccess)
	{
		SteamGameServer_Shutdown();
	}
	singleton = NULL;
}