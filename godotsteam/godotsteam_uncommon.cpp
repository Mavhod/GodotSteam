
#include "godotsteam_uncommon.h"

#include "core/io/ip_address.h"
#include "core/io/ip.h"

SteamUC* SteamUC::singleton = NULL;

SteamUC::SteamUC()
{
	singleton = this;
}

SteamUC* SteamUC::get_singleton() { return singleton; }





bool SteamUC::is_running()
{
	return SteamAPI_IsSteamRunning();
}

// returns app ID
int SteamUC::get_appid()
{
	if ( SteamUtils() == NULL ) { return 0; }
	return SteamUtils()->GetAppID();
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		SERVER/GAME INFO
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Set data to be replicated to friends so that they can join your game
void SteamUC::set_fake_server_info(const String& server_ip, int port)
{
	if ( SteamUser() == NULL ) { return; }
	// resolve address and convert it to int (IP_Address) union
	IP_Address addr(IP::get_singleton()->resolve_hostname(server_ip));
	// for some reason bytes are in wrong order, need to swap them
	for(int i=0;i<2;i++)
	{
		uint8 temp = addr.field[i];
		addr.field[i] = addr.field[3-i];
		addr.field[3-i] = temp;
	}
	CSteamID gameserverID = SteamUser()->GetSteamID(); // faking server ID
	SteamUser()->AdvertiseGame(gameserverID, addr.host, port); 
}

/*	Rich Presence data is automatically shared between friends who are in the same game
Each user has a set of Key/Value pairs
Up to 20 different keys can be set
There are two magic keys:
		"status"  - a UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list
		"connect" - a UTF-8 string that contains the command-line for how a friend can connect to a game
set_game_info() to an empty string deletes the key	*/
void SteamUC::set_game_info(const String& s_key, const String& s_value)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->SetRichPresence(s_key.utf8().get_data(),s_value.utf8().get_data());
}

// Cleans up all the keys
void SteamUC::clear_game_info()
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ClearRichPresence();
}







/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		OVERLAY
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
// start & hook the game process, so this function will initially return false while the overlay is loading.
bool SteamUC::overlay_is_enabled()
{
	if ( SteamUtils() == NULL ) { return false; }
	return SteamUtils()->IsOverlayEnabled();
}

// Change position(corner) at which notifications (messages, etc.) will be displayed
// TOP_LEFT=0 | TOP_RIGHT=1 | BOT_LEFT=2 | BOT_RIGHT=3
void SteamUC::overlay_set_notify_pos( int pos )
{
	if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) { return; }
	SteamUtils()->SetOverlayNotificationPosition( ENotificationPosition(pos) ) ;
}

// activates the game overlay, with an optional dialog to open 
// valid options are "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements"
void SteamUC::overlay_open(const String& url)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlay( url.utf8().get_data() );
}

// activates game overlay to a specific place
// valid options are
//		"steamid" - opens the overlay web browser to the specified user or groups profile
//		"chat" - opens a chat window to the specified user, or joins the group chat 
//		"jointrade" - opens a window to a Steam Trading session that was started with the ISteamEconomy/StartTrade Web API
//		"stats" - opens the overlay web browser to the specified user's stats
//		"achievements" - opens the overlay web browser to the specified user's achievements
//		"friendadd" - opens the overlay in minimal mode prompting the user to add the target user as a friend
//		"friendremove" - opens the overlay in minimal mode prompting the user to remove the target friend
//		"friendrequestaccept" - opens the overlay in minimal mode prompting the user to accept an incoming friend invite
//		"friendrequestignore" - opens the overlay in minimal mode prompting the user to ignore an incoming friend invite

// Open URL using Steam overlay browser
void SteamUC::overlay_open_url(const String& url)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToWebPage( url.utf8().get_data() );
}

// Opens Steam App Store
void SteamUC::overlay_open_store(int app_id)
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ActivateGameOverlayToStore( AppId_t(app_id), EOverlayToStoreFlag(0) );
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		SOCIAL
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Returns Array of Dictionaries containing groups data which user belongs to
// 		{"id":int, "name":string, "tag":string}
Array SteamUC::get_groups()
{
	if ( SteamFriends() == NULL ) { return Array(); }
	int groupCount = SteamFriends()->GetClanCount();
	Array groups;
	for(int i=0;i<groupCount;i++)
	{
		Dictionary group;
		CSteamID groupId = SteamFriends()->GetClanByIndex( i );
		String gName = SteamFriends()->GetClanName( groupId );
		String gTag = SteamFriends()->GetClanTag( groupId );
		group["id"] = groupId.GetAccountID();
		group["name"] = gName;
		group["tag"] = gTag;
		groups.append(group);
	}
	return groups;
}

// Returns Array of Dictionaries containing your friends data
// 		{"id":int, "name":string, "status":int(see constants)}
Array SteamUC::get_friends()
{
	if ( SteamFriends() == NULL ) { return Array(); }
	int fCount = SteamFriends()->GetFriendCount( 0x04 );
	Array friends;
	for(int i=0;i<fCount;i++)
	{
		Dictionary friendd;
		CSteamID friendId = SteamFriends()->GetFriendByIndex( i, 0x04 );
		String fName = SteamFriends()->GetFriendPersonaName( friendId );
		int fStatus = SteamFriends()->GetFriendPersonaState( friendId );
		friendd["id"] = friendId.GetAccountID();
		friendd["name"] = fName;
		friendd["status"] = fStatus;
		friends.append(friendd);
	}
	return friends;
}

// Invites user(friend) to join your game
// If player is already in-game: "join_requested" signal will be emitted, containing:
// 			(int)steam_id and connect_string
void SteamUC::invite_friend( int steamId, const String& conString )
{
	if ( SteamFriends() == NULL ) { return; }
	CSteamID friendId = create_steamid( steamId );
	SteamFriends()->InviteUserToGame( friendId, conString.utf8().get_data() );
}

// Marks player as "played with". You can retrieve those with 'get_recent_players()'
void SteamUC::set_played_with(int steamId)
{
	if ( SteamFriends() == NULL ) { return; }
	CSteamID friendId = create_steamid( steamId );
	SteamFriends()->SetPlayedWith( friendId );
}

// returns players which were marked as "played with" recently in this game
Array SteamUC::get_recent_players()
{
	if ( SteamFriends() == NULL ) { return Array(); }
	int rCount = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i=0;i<rCount;i++)
	{
		CSteamID rPlayerId = SteamFriends()->GetCoplayFriend( i );
		if ( SteamFriends()->GetFriendCoplayGame( rPlayerId ) == SteamUtils()->GetAppID() )
		{
			Dictionary rPlayer;
			String rName = SteamFriends()->GetFriendPersonaName( rPlayerId );
			int rStatus = SteamFriends()->GetFriendPersonaState( rPlayerId );
			rPlayer["id"] = rPlayerId.GetAccountID();
			rPlayer["name"] = rName;
			rPlayer["status"] = rStatus;
			recents.append(rPlayer);
		}
	}
	return recents;
}









/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//		SIGNALS
//
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SteamUC::_server_connected( SteamServersConnected_t* conData )
	{ emit_signal("connection_changed",true); }
void SteamUC::_server_disconnected( SteamServersDisconnected_t* conData )
	{ emit_signal("connection_changed",false); }

void SteamUC::_overlay_toggled( GameOverlayActivated_t* callData )
{
	if (callData->m_bActive)
		{ emit_signal("overlay_toggled",true); }
	else
		{ emit_signal("overlay_toggled",false); }
}

void SteamUC::_join_requested( GameRichPresenceJoinRequested_t* callData )
{
	int steamId = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	emit_signal("join_requested", steamId, con_string);
}







void SteamUC::_bind_methods()
{
	ObjectTypeDB::bind_method("is_steam_running",&SteamUC::is_running);
	ObjectTypeDB::bind_method("get_appid",&SteamUC::get_appid);
	// Server/Game info
	ObjectTypeDB::bind_method(_MD("set_fake_server_info","server_ip","port"),&SteamUC::set_fake_server_info);
	ObjectTypeDB::bind_method(_MD("set_game_info","key","value"),&SteamUC::set_game_info);
	ObjectTypeDB::bind_method(_MD("clear_game_info"),&SteamUC::clear_game_info);
	// Overlay
	ObjectTypeDB::bind_method("overlay_is_enabled",&SteamUC::overlay_is_enabled);
	ObjectTypeDB::bind_method(_MD("overlay_set_notification_pos","0-3"),&SteamUC::overlay_set_notify_pos);
	ObjectTypeDB::bind_method(_MD("overlay_open","type"),&SteamUC::overlay_open,DEFVAL(""));
	ObjectTypeDB::bind_method(_MD("overlay_open_url","url"),&SteamUC::overlay_open_url);
	ObjectTypeDB::bind_method(_MD("overlay_open_store","appID"),&SteamUC::overlay_open_store,DEFVAL(0));
	// Social
	ObjectTypeDB::bind_method("get_groups",&SteamUC::get_groups);
	ObjectTypeDB::bind_method("get_friends",&SteamUC::get_friends);
	ObjectTypeDB::bind_method(_MD("invite_friend","steam_id","connect_string"),&SteamUC::invite_friend);
	ObjectTypeDB::bind_method(_MD("set_played_with","steam_id"),&SteamUC::set_played_with);
	ObjectTypeDB::bind_method("get_recent_players",&SteamUC::get_recent_players);
	
	
	
	
	ADD_SIGNAL(MethodInfo("connection_changed",PropertyInfo(Variant::BOOL,"connected")));
	ADD_SIGNAL(MethodInfo("overlay_toggled",PropertyInfo(Variant::BOOL,"active")));
	ADD_SIGNAL(MethodInfo("join_requested",PropertyInfo(Variant::INT,"from"),PropertyInfo(Variant::STRING,"connect_string")));
	
	BIND_CONSTANT(TOP_LEFT);
	BIND_CONSTANT(TOP_RIGHT);
	BIND_CONSTANT(BOT_LEFT);
	BIND_CONSTANT(BOT_RIGHT);
	
	BIND_CONSTANT(OFFLINE); // 0
	BIND_CONSTANT(ONLINE); // 1
	BIND_CONSTANT(BUSY); // ...
	BIND_CONSTANT(AWAY);
	BIND_CONSTANT(SNOOZE);
	BIND_CONSTANT(LF_TRADE);
	BIND_CONSTANT(LF_PLAY);	
}

SteamUC::~SteamUC()
{
	singleton = NULL;
}
