
#ifndef GSTEAMSTATS_H
#define GSTEAMSTATS_H

#include "object.h"
#include "dictionary.h" // contains array.h as well

#include <steam/steam_api.h>

class SteamStats: public Object
{
public:
	// enum {
		
	// };
	static SteamStats* get_singleton();
	SteamStats();
	~SteamStats();
	
	bool request_stats();
	bool store_stats();
	
	int get_achievement(const String& aName);
	int get_stat(const String& sName);
	

protected:
	static void _bind_methods();
	static SteamStats* singleton;

private:
	bool isInitialized;
	STEAM_CALLBACK(SteamStats,_stats_received,UserStatsReceived_t);
	STEAM_CALLBACK(SteamStats,_stats_stored,UserStatsStored_t);
		
	OBJ_TYPE(SteamStats, Object);
	OBJ_CATEGORY("Steam");
};
#endif


