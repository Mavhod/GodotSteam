
#ifndef GSTEAMUSER_H
#define GSTEAMUSER_H

#include "gsteamid.h"



class _SteamUser: public SteamID
{
	friend class Steam;
public:
	enum{
		INVALID=0,YOU=1,USER=2,
		USER_TYPE_MAX,
		
		REL_NONE=0,REL_BLOCKED=1,REL_REQUEST_REC=2,REL_FRIEND=3,REL_REQUEST_INIT=4,REL_IGNORED=5,REL_IGNORED_USER=6,REL_SUGGESTED=7,
		REL_MAX,
		
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
	};
	_SteamUser() { }
	_SteamUser(CSteamID cSID); // it will also try to find the type. If needed you can override type with setUserType()
	//_SteamUser(CSteamID cSID, int uType) { cSteamID = cSID; setUserType(uType); }
	

	// for multiple user types
	int get_user_type() { return user_type; }
	String get_name();
	int get_state();
	int get_steamlevel();
	String get_rich_presence(const String& s_key);
	// YOU
	bool is_logged();
	bool set_rich_presence(const String& s_key, const String& s_value);
	bool clear_rich_presence();
	// USER
	bool is_friend();
		int get_relationship(); // not bound, probably only useful for stuff like invitations, messages. Check REL_ constants
	

protected:
	int user_type=0;
	void setUserType(int uType); // sets to INVALID if this type is undeclared
	void _steamid_changed() { updateType(); }
	bool updateType(); // checks&update the type. Returns true if type changed. Mostly used when `cSteamID` changed
	static void _bind_methods();

private:
	OBJ_TYPE(_SteamUser, SteamID);
};
#endif


