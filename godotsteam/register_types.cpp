
/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"

#include "godotsteam.h"
#include "gsteamgameserver.h"

#include "globals.h"

static Steam* SteamPtr = NULL;
static _SteamGameServer* SteamGServer = NULL;
static SteamStats* SteamStatsPtr = NULL;


void register_godotsteam_types()
{
	SteamPtr = memnew(Steam);
	SteamGServer = memnew(_SteamGameServer);
	SteamStatsPtr = memnew(SteamStats);
	
	Globals::get_singleton()->add_singleton(Globals::Singleton("Steam",Steam::get_singleton()));
	Globals::get_singleton()->add_singleton(Globals::Singleton("SteamGameServer",_SteamGameServer::get_singleton()));
	Globals::get_singleton()->add_singleton(Globals::Singleton("SteamStats",SteamStats::get_singleton()));
	
	
	ObjectTypeDB::register_virtual_type<Steam>();
	ObjectTypeDB::register_virtual_type<_SteamGameServer>();
	ObjectTypeDB::register_virtual_type<SteamStats>();
	
	//ObjectTypeDB::register_type<SteamID>();
	ObjectTypeDB::register_virtual_type<_SteamUser>();
	ObjectTypeDB::register_virtual_type<_SteamGroup>();
}

void unregister_godotsteam_types()
{
	memdelete(SteamPtr);
	memdelete(SteamGServer);
	memdelete(SteamStatsPtr);
}
