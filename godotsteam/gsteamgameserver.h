
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
	
	
	bool init(const String& ipAddress, int steamPort, int gamePort, int queryPort, int serverMode);
	void logon(const String& token);
	void logon_anon();
	void logoff();
	bool is_logged();
	String get_ip();
	Ref<SteamID> get_steam_id();
	
	void set_product(const String& pName);
	void set_description(const String& sDesc);
	void enable_heartbeats(bool active);
	void set_max_players( int pCount );
	void set_server_name( const String& sName );
	
	

protected:
	static void _bind_methods();
	static _SteamGameServer* singleton;

private:
	bool isInitSuccess;
	const char* serverVersion="0.0.0.1"; // The version string is usually in the form x.x.x.x, and is used by the master server to detect when the server is out of date.  (Only servers with the latest version will be listed.)
	Ref<SteamID> serverSteamID=NULL;
	OBJ_TYPE(_SteamGameServer, Object);
	OBJ_CATEGORY("Steam");
};
#endif


