
/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"

#include "godotsteam.h"
#include "godotsteam_uncommon.h"

#include "globals.h"

static Steam* SteamPtr = NULL;
static SteamUC* SteamUCPtr = NULL;


void register_godotsteam_types()
{
	SteamPtr = memnew(Steam);
	SteamUCPtr = memnew(SteamUC);
	
	Globals::get_singleton()->add_singleton(Globals::Singleton("Steam",Steam::get_singleton()));
	Globals::get_singleton()->add_singleton(Globals::Singleton("SteamUC",SteamUC::get_singleton()));
	
	
	ObjectTypeDB::register_virtual_type<Steam>();
	ObjectTypeDB::register_virtual_type<SteamUC>();
}

void unregister_godotsteam_types()
{
	memdelete(SteamPtr);
	memdelete(SteamUCPtr);
}
