
#ifndef GSTEAMGROUP_H
#define GSTEAMGROUP_H

#include "gsteamid.h"



class _SteamGroup: public SteamID
{
public:
	// enum{

	// };
	_SteamGroup() { }
	_SteamGroup(CSteamID cSID);
	

	String get_name();
	String get_tag();
	bool has_tag();
	
	void open_chat();
	

protected:
	static void _bind_methods();
	void _steamid_changed() {}

private:
	OBJ_TYPE(_SteamGroup, SteamID);
};
#endif


