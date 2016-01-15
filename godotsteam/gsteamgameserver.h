
#ifndef GSTEAMGAMESERVER_H
#define GSTEAMGAMESERVER_H

#include "object.h"
#include "gsteamid.h"

class _SteamGameServer: public Object
{
public:
	enum{
		INVALID=0,NO_AUTH=1,AUTH=2,AUTH_VAC=3,
	};
	static _SteamGameServer* get_singleton();
	_SteamGameServer();
	~_SteamGameServer();
	
	
	
	

protected:
	static void _bind_methods();
	static _SteamGameServer* singleton;

private:
	bool isInitSuccess;
	const String serverVersion="0.0.0.1"; // The version string is usually in the form x.x.x.x, and is used by the master server to detect when the server is out of date.  (Only servers with the latest version will be listed.)
	Ref<SteamID> serverSteamID=NULL;
	OBJ_TYPE(_SteamGameServer, Object);
	OBJ_CATEGORY("Steam");
};
#endif


