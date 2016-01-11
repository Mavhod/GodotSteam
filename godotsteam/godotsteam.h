
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "object.h"
#include "dictionary.h" // contains array.h as well
#include "steamuser.h"
#include "steamfriend.h"

class Steam: public Object
{
public:
	enum {
		TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
		
		UNIVERSE_INVALID=0, UNIVERSE_PUBLIC=1, UNIVERSE_BETA=2, UNIVERSE_INTERNAL=3, UNIVERSE_DEV=4,
		
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, MAX, NOT_OFFLINE=8, ALL=9
	};
	static Steam* get_singleton();
	Steam();
	~Steam();
	
	Ref<_SteamUser> get_user();
	
	bool init();
	bool is_steam_running();
	int get_appid();
	String get_userdata_path();
	
//	void set_username(const String& new_name);
	void user_set_server_info(int host_steamid, const String& server_ip, int port);
	void user_clear_rich_presence();
	bool user_is_friend(int steamid);
	
	Array friends_getall( int filter=NOT_OFFLINE );
	
	bool overlay_is_enabled();
	void overlay_set_notify_pos(int pos);
	void overlay_open(const String& type);
	void overlay_open_user(const String& type, Ref<SteamID> steamid);
	void overlay_open_url(const String& url);
	void overlay_open_store(int appid=0);

protected:
	static void _bind_methods();
	static Steam* singleton;

private:
	bool isInitSuccess;
	OBJ_TYPE(Steam, Object);
	OBJ_CATEGORY("Steam");
};
#endif // GODOTSTEAM_H


