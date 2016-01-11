
#ifndef GSTEAMFRIEND_H
#define GSTEAMFRIEND_H

#include "steamuser_base.h"

class SteamFriend: public SteamID
{
public:
	enum{
		REL_NONE=0,REL_BLOCKED=1,REL_REQUEST_REC=2,REL_FRIEND=3,REL_REQUEST_INIT=4,REL_IGNORED=5,REL_IGNORED_FRIEND=6,REL_SUGGESTED=7,
	};
	SteamFriend(CSteamID cSID) { cSteamID = cSID; }
	
	String get_name(); // override
	int get_state(); // override
	int get_steamlevel(); // override
	bool is_friend();
	int get_relationship();

protected:
	static void _bind_methods();
	
private:
	OBJ_TYPE(SteamFriend, SteamID);
};
#endif


