
#ifndef GSTEAMID_H
#define GSTEAMID_H

#include "reference.h"
#include <steam/steam_api.h>


class SteamID: public Reference
{
	friend class Steam;
	
public:
	enum {
		//OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE=5, LF_PLAY=6, MAX=7, NOT_OFFLINE=8, ALL=9,
		UNIVERSE_INVALID=0, UNIVERSE_PUBLIC=1, UNIVERSE_BETA=2, UNIVERSE_INTERNAL=3, UNIVERSE_DEV=4,
		UNIVERSE_MAX,
		
		TYPE_INVALID=0,TYPE_INDIVIDUAL=1,TYPE_MULTISEAT=2,TYPE_GAMESERVER=3,TYPE_ANON_GAMESERVER=4,TYPE_PENDING=5,TYPE_CONTENTSERVER=6,TYPE_GROUP=7,TYPE_CHAT=8,TYPE_CONSOLEUSER=9,TYPE_ANON_USER=10, 
		TYPE_MAX,
	};
	SteamID() {}
	SteamID(CSteamID cSID) { setCSteamID(cSID); }
	SteamID(uint64 steamID) { setID(steamID); }
	
	CSteamID getCSteamID() { return cSteamID; } // return copy
	void setCSteamID( CSteamID cSID ) { cSteamID=cSID; }
	void setID( uint64 steamid ) { cSteamID.SetFromUint64(steamid); }
	
	// GDScript
	bool is_valid();
	int get_steamID();
	int get_accountID();
	int get_universe();
	int get_account_type();
		//void set_steamID(int steamid) { setID(steamid); } // unbound, recommended not to use
	
	String get_profile_url(); // just for users and groups

protected:
	static void _bind_methods();
	CSteamID cSteamID;
	OBJ_CATEGORY("Steam");

private:
	OBJ_TYPE(SteamID, Reference);
};
#endif
