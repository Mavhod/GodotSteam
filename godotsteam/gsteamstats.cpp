
#include "gsteamstats.h"
#include <steam/steam_api.h>

#include "godotsteam.h"

SteamStats* SteamStats::singleton = NULL;

SteamStats::SteamStats()
{
	singleton = this;
	isInitialized = false;
	// try to initialize, might fail
	request_stats();
	printf("Stats initialized: %d\n",isInitialized);
}

SteamStats* SteamStats::get_singleton() { return singleton; }

void SteamStats::_stats_received(UserStatsReceived_t* callData)
{
	if ( SteamUser() == NULL ) { return; }
	if ( callData->m_nGameID == Steam::get_singleton()->get_appid() )
	{
		if ( callData->m_eResult == k_EResultOK )
		{
			CSteamID targetID = callData->m_steamIDUser;
			if ( targetID == Steam::get_singleton()->get_user()->getCSteamID() )
			{
				if (!isInitialized)
				{
					isInitialized = true;
				}
			}
			else // some other user stats
			{
				
			}
		}
		else
		{
			printf("[Steam]request_stats returned error (%d)\n",callData->m_eResult);
		}
		// temp
		emit_signal("stats_received", callData->m_eResult);
	}
}

void SteamStats::_stats_stored(UserStatsStored_t* callData)
{
	if ( SteamUser() == NULL ) { return; }
	if ( callData->m_nGameID == Steam::get_singleton()->get_appid() )
	{
		if ( callData->m_eResult == k_EResultOK )
		{
			// success storing stats
		}
		else
		{
			printf("[Steam]store_stats returned error (%d)\n",callData->m_eResult);
		}
		emit_signal("stats_stored", callData->m_eResult);
	}
}
	
bool SteamStats::request_stats()
{
	if ( SteamUserStats() == NULL || SteamUser() == NULL )
		{ return false; }
	if ( !SteamUser()->BLoggedOn() )
		{ return false; }
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamStats::store_stats()
{
	if ( SteamUserStats() == NULL || SteamUser() == NULL )
		{ return false; }
	if ( !SteamUser()->BLoggedOn() )
		{ return false; }
	return SteamUserStats()->StoreStats();
}

int SteamStats::get_achievement(const String& aName)
{
	if ( SteamUserStats() == NULL || !isInitialized ) { return -1; } // error
	bool unlocked;
	if ( SteamUserStats()->GetAchievement(aName.utf8().get_data(),&unlocked) == false )
		{ return -1; } // error, wrong name?
	return unlocked;
}

int SteamStats::get_stat(const String& sName)
{
	if ( SteamUserStats() == NULL || !isInitialized ) { return -1; } // error
	int32 value;
	if ( SteamUserStats()->GetStat(sName.utf8().get_data(),&value) == false )
		{ return -1; } // error, wrong name?
	return value;
}


void SteamStats::_bind_methods()
{
    ObjectTypeDB::bind_method("request_stats",&SteamStats::request_stats);
	ObjectTypeDB::bind_method("store_stats",&SteamStats::store_stats);
	
	ObjectTypeDB::bind_method("get_stat",&SteamStats::get_stat);
	ObjectTypeDB::bind_method("get_achievement",&SteamStats::get_achievement);
	
	
	ADD_SIGNAL(MethodInfo("stats_received",PropertyInfo(Variant::INT,"error")));
	ADD_SIGNAL(MethodInfo("stats_stored",PropertyInfo(Variant::INT,"error")));
	
	// BIND_CONSTANT();
}

SteamStats::~SteamStats()
{
	singleton = NULL;
}
