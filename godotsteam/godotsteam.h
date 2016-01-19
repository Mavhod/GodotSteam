
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "object.h"
#include "dictionary.h" // contains array.h as well
#include "gsteamuser.h"
#include "gsteamgroup.h"
#include "gsteamstats.h"


class Steam: public Object
{
public:
	enum {
		TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
		
		UNIVERSE_INVALID=0, UNIVERSE_PUBLIC=1, UNIVERSE_BETA=2, UNIVERSE_INTERNAL=3, UNIVERSE_DEV=4,
		
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
		
		ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
	};
	static Steam* get_singleton();
	Steam();
	~Steam();
	
	
	Ref<_SteamUser> get_user();
	
	int init();
	bool start_via_steam();
		// bool is_running();
		// bool got_connection();
	int get_appid();
	String get_userdata_path();
	
//	void set_username(const String& new_name);
	void user_set_server_info(Ref<SteamID> gameserver, const String& server_ip, int port);
	bool set_game_info(const String& s_key, const String& s_value);
	void clear_game_info();
	
	Array friends_getall( int filter=NOT_OFFLINE );
	Array groups_getall();
	Array get_recent_players();
	
	bool overlay_is_enabled();
	void overlay_set_notify_pos(int pos);
	void overlay_open(const String& type);
	void overlay_open_user(const String& type, Ref<SteamID> steamid);
	void overlay_open_url(const String& url);
	void overlay_open_store(int appid=0);

protected:
	static void _bind_methods();
	static Steam* singleton;
	void updateFriendList(int filter=NULL); // default = last used filter
	void updateGroupList();

private:
	uint64 fixedAppID=0;
	STEAM_CALLBACK(Steam,_server_connected,SteamServersConnected_t);
	STEAM_CALLBACK(Steam,_server_disconnected,SteamServersDisconnected_t);
	STEAM_CALLBACK(Steam,_overlay_toggled,GameOverlayActivated_t);
	// temporary (i hope)
	void run_callbacks() { SteamAPI_RunCallbacks(); }
	//

	bool isInitSuccess;
	Ref<_SteamUser> yourUser = NULL;
	Array friendList;
	Array groupList;
	int lastFriendsFilter=ALL;
	OBJ_TYPE(Steam, Object);
	OBJ_CATEGORY("Steam");
};
#endif


