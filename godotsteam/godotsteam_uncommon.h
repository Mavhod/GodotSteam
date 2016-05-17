
#ifndef GODOTSTEAM_UNCOMMON_H
#define GODOTSTEAM_UNCOMMON_H

#include <steam/steam_api.h>

#include "object.h"
#include "dictionary.h" // contains array.h as well

#include "godotsteam.h"

class SteamUC: public Object
{
friend Steam;
public:
	enum {
		TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
		
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX
		
	};
	static SteamUC* get_singleton();
	SteamUC();
	~SteamUC();
	
	
	bool is_running();
	int get_appid();
//	void set_username(const String& new_name);
	// Server/Game info
	void set_fake_server_info(const String& server_ip, int port);
	void set_game_info(const String& s_key, const String& s_value);
	void clear_game_info();
	// Overlay
	bool overlay_is_enabled();
	void overlay_set_notify_pos(int pos);
	void overlay_open(const String& type);
	void overlay_open_url(const String& url);
	void overlay_open_store(int appid=0);
	// Social
	Array get_groups();
	Array get_friends();
	void invite_friend(int id, const String& conString);
	void set_played_with(int steam_id);
	Array get_recent_players();
	CSteamID create_steamid( uint32 steamId, int accountType=-1 ) { return Steam::get_singleton()->create_steamid( steamId, accountType ); }
	

protected:
	static void _bind_methods();
	static SteamUC* singleton;

private:
	STEAM_CALLBACK(SteamUC,_server_connected,SteamServersConnected_t);
	STEAM_CALLBACK(SteamUC,_server_disconnected,SteamServersDisconnected_t);
	STEAM_CALLBACK(SteamUC,_overlay_toggled,GameOverlayActivated_t);
	STEAM_CALLBACK(SteamUC,_join_requested,GameRichPresenceJoinRequested_t);

	OBJ_TYPE(SteamUC, Object);
	OBJ_CATEGORY("Steam");
	
};
#endif


