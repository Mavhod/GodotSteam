
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "object.h"
//#include "globals.h"

class Steam: public Object
{
public:
	enum {
		TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
		
		UNIVERSE_INVALID=0, UNIVERSE_PUBLIC=1, UNIVERSE_BETA=2, UNIVERSE_INTERNAL=3, UNIVERSE_DEV=4,
	};
	static Steam* get_singleton();
	Steam();
	~Steam();
	
	bool init();
	bool is_steam_running();
	int get_appid();
	String get_userdata_path();
	
	bool user_is_logged();
	int user_get_id();
	int user_get_account_id();
	int user_get_universe();
	String user_get_name();
	int user_get_steam_level();
	String user_get_profile_link();
//	void set_username(const String& new_name);
	void user_set_server_info(int host_steamid, const String& server_ip, int port);
	
	bool overlay_is_enabled();
	void overlay_set_notify_pos(int pos);
//	void overlay_open(const String& type);
	void overlay_open_url(const String& url);

protected:
	static void _bind_methods();
	static Steam* singleton;

private:
	bool isInitSuccess;
	OBJ_TYPE(Steam, Object);
	OBJ_CATEGORY("Steam");
};
#endif // GODOTSTEAM_H


