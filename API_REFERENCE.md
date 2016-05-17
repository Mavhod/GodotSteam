# API Reference
Steam API for Godot engine.

Steam
----------
int **init**()
```
	Needs to be called in order for this module to work properly. Returns:
		OK - if everything initialized properly
		FAILED - if initialization failed, nothing will work
		ERR_NO_CLIENT - if there is no Steam client running, probably nothing will work
		ERR_NO_CONNECTION - initialized, but can't connect to Steam servers.
			Most of functionality will be restored once you connect (check "connection_changed" signal)
```
void **run_callbacks**()
```
	This is required for many functions to work properly (f.e. for users to emit "avatar_loaded" signal).
	It should be called at least few times a second. Recommended to use it inside "_process".
```
String **get_userdata_path**()
```
	Returns steam userdata path ( something like "C:\Progam Files\Steam\userdata\<SteamID>\<AppID>\local" ).
```
String **get_username**( int steamID=0 )
```
	Returns current user name. If steamID is given and bigger than 0, it'll TRY TO find name of specific user.
```
int **get_steamid**()
```
	Returns account ID.
```
void **unlock_achiv**( String api_name )
```
	Unlocks specified achievement for the player. You setup API_Name using Steamworks website.
```
bool **has_achiv**( String api_name )
```
	Checks if player has unlocked specified achievement.
```
void **set_stat_i**( String api_name, int value )
void **set_stat_f**( String api_name, float value )
```
	Sets the value of specified stat.
```
int **get_stat_i**( String api_name )
float **get_stat_f**( String api_name )
```
	Returns the value of specified stat.
```
void **sync_stats**()
```
	Sends your stats&achievements to the Steam and requests current values from it.
	Recommended to call it from time-to-time.
```
void **load_avatar**( int size=AVATAR_MEDIUM )
```
	Loads current avatar from Steam. Use Steam.connect("avatar_loaded" ... ) before calling it.
	When it loads "avatar_loaded" signal is emitted containing: avatar size(int) and avatar(Image).
	Constants:
		AVATAR_SMALL=0 [32px] | AVATAR_MEDIUM=1 [64px] | AVATAR_LARGE=2 [184px]
```




SteamUC
----------
bool **is_steam_running**()
```
	Checks if Steam process is running. This does not check if .init worked or you're connected to Steam servers.
	Most likely it just checks if there's active Steam.exe process.
```
int **get_appid**()
```
	Returns game Steam AppID
```
void **set_fake_server_info**( String server_ip, int port )
```
	Updates info about server you are playing on. This IP&port should show up when someone will try to 
	join you or view info about your game.
```
void **set_game_info**( String key, String value )
```
	This data is automatically shared between friends who are in the same game
	Each user has a set of Key/Value pairs. Up to 20 different keys can be set
	There are two magic keys:
			"status"  - string that will show up in the 'view game info' dialog in the Steam friends list
			"connect" - string that contains the command-line for how a friend can connect to a game
	Use empty string "" to erase the key.
```
bool **clear_game_info**()
```
	Erases all pairs set.
```
bool **overlay_is_enabled**()
```
	Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds
	to start & hook the game process, so this function will initially return false while the overlay is loading.
```
void **overlay_set_notification_pos**( int pos(check constants) )
```
	Change position(corner) at which notifications (messages, etc.) will be displayed.
	Constants: TOP_LEFT=0 | TOP_RIGHT=1 | BOT_LEFT=2 | BOT_RIGHT=3
```

void **overlay_open**( String type="" )
```
	Opens Steam overlay. Optional types are (not case-sensitive), not all tested:
	"Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements".
```
void **overlay_open_url**( String url )
```
	Opens URL using Steam overlay web browser.
```
void **overlay_open_store**( int appID=0 )
```
	Opens specified app store site. If invalid or none given, main Steam store site will open.
```
Array **get_friends**()
```
	Returns Array of Dictionaries containing your friends data:
		{ "id":steam_id(int), "name":string, "status":int(check constants) }
	OFFLINE=0 | ONLINE=1 | BUSY=2 | AWAY=3 | SNOOZE=4 | LF_TRADE=5 | LF_PLAY=6
```

Array **get_groups**()
```
	Returns Array of Dictionaries containing you groups data:
		{ "id":steam_id(int), "name":string, "tag":string }
```
void **invite_friend**( int steam_id )
```
	Invites user(friend) to join your game.
	If player is already in-game: "join_requested" signal will be emitted, containing:
		steam_id and connect_string
```
void **set_played_with**( int steam_id )
```
	Marks player as "played with". You can retrieve "played with" players using 'get_recent_players()'.
```
Array **get_recent_players**()
```
	Returns Array of Dictionaries containing players you recently played this game with.
		{ "id":steam_id(int), "name":string, "status":int(check constants) }
	OFFLINE=0 | ONLINE=1 | BUSY=2 | AWAY=3 | SNOOZE=4 | LF_TRADE=5 | LF_PLAY=6
```  




  **Signals:**  
  
**overlay_toggled**( bool active )
```
	Emmited when Steam overlay is opened or closed.
```
**connection_changed**( bool connected )
```
	Emmited when connection to Steam servers is lost or renewed.
```
**join_requested**( int steam_id, String connect_string )
```
	Emmited when received invitation (while being in-game ofc.). steam_id is ID of a friend from which you
	received it. Could be empty if he's not on your friendlist.
```