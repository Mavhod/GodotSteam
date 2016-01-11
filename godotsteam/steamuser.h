
#ifndef GSTEAMUSER_H
#define GSTEAMUSER_H

#include "steamuser_base.h"

class _SteamUser: public SteamID
{
public:
	_SteamUser(CSteamID cSID) { cSteamID = cSID; }
	
	String get_name(); // override
	int get_state(); // override
	int get_steamlevel(); // override
	bool is_logged();
	bool set_rich_presence(const String& s_key, const String& s_value);
	void clear_rich_presence();
	

protected:
	static void _bind_methods();

private:
	OBJ_TYPE(_SteamUser, SteamID);
};
#endif


