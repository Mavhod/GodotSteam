
#ifndef GSTEAMUSERBASE_H
#define GSTEAMUSERBASE_H

#include "reference.h"
#include <steam/steam_api.h>

class SteamID: public Reference
{
public:
	enum {
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE=5, LF_PLAY=6, MAX=7, NOT_OFFLINE=8, ALL=9,
		UNIVERSE_INVALID=0, UNIVERSE_PUBLIC=1, UNIVERSE_BETA=2, UNIVERSE_INTERNAL=3, UNIVERSE_DEV=4,
	};
	SteamID(){}
	SteamID(CSteamID cSID);
	SteamID(uint64 steamID);
	
	CSteamID get_CSteamID() { return cSteamID; }
	
	int get_steamID();
	int get_accountID();
	int get_universe();
	virtual String get_name()=0;
	virtual int get_state()=0;
	virtual int get_steamlevel()=0;
	String get_rich_presence(const String& s_key);
	String get_profile_url();

protected:
	static void _bind_methods();
	CSteamID cSteamID;
	OBJ_CATEGORY("Steam");

private:
	OBJ_TYPE(SteamID, Reference);
};
#endif


