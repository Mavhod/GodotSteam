
#ifndef GSTEAMUSER_H
#define GSTEAMUSER_H

#include "gsteamid.h"
#include "scene/resources/texture.h" // avatars



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
		
		AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE
	};
	explicit _SteamUser() { }
	_SteamUser(CSteamID cSID); // it will also try to find the type. If needed you can override type with setUserType()
	//_SteamUser(CSteamID cSID, int uType) { cSteamID = cSID; setUserType(uType); }
	~_SteamUser();
	

	// for multiple user types
	int get_user_type() { return user_type; }
	String get_name();
	int get_state();
	int get_steamlevel();
	String get_game_info(const String& s_key);
	bool load_avatar(int size=AVATAR_MEDIUM);
	// YOU
	bool is_logged(); // moved to 'Steam'
	// USER
	bool is_friend();
		int get_relationship(); // not bound, probably only useful for stuff like invitations, messages. Check REL_ constants
	void request_game_info();
	void set_played_with();
	

protected:
	int user_type=0;
	void setUserType(int uType); // sets to INVALID if this type is undeclared
	bool updateType(); // checks&update the type. Returns true if type changed. Mostly used when `cSteamID` changed
	// void _game_info_received( FriendRichPresenceUpdate_t* rich_update );
	static void _bind_methods();

private:
	void _steamid_changed() { updateType(); }
	Image draw_avatar(int size, uint8* buffer);
	OBJ_TYPE(_SteamUser, SteamID);
	
	
	// CALLBACKS TEST
	STEAM_CALLBACK(_SteamUser, _game_info_received, FriendRichPresenceUpdate_t );
	STEAM_CALLBACK(_SteamUser, _avatar_loaded, AvatarImageLoaded_t );
	
	
	// CCallResult<_SteamUser, FriendRichPresenceUpdate_t> callRichPresenceUpdated;
		// struct FriendRichPresenceUpdate_t
		//	CSteamID m_steamIDFriend;	// friend who's rich presence has changed
		//	AppId_t m_nAppID;			// the appID of the game (should always be the current game)
	// ---
};
#endif


