# GodotSteam
Steam api for Godot game engine.

For Windows and Linux platform.


How to use
----------
- Download [Steamworks SDK](https://partner.steamgames.com).
- Copy:

  ```
  sdk/public
  sdk/redistributable_bin
  ```
  To
  ```
  godotsteam/sdk
  ```
  The directory contents should look like this:
  ```
  godotsteam/sdk/public/*
  godotsteam/sdk/redistributable_bin/*
  godotsteam/SCsub
  godotsteam/config.py
  godotsteam/godotsteam.cpp
  godotsteam/godotsteam.h
  godotsteam/register_types.cpp
  godotsteam/register_types.h
  ```
- Drop the "godotsteam" directory inside the "modules" directory on the Godot source. Recompile for your platform.

  For Linux you must add ```openssl=no``` when compile because it has problem with lib crypto (class StreamPeerSSL may can't use).
  
  For Windows it's highly advised to use Visual Studio. With some extra steps MinGW will work, but most likely with limited functionality.

- Copy shared library (steam_api) to godot binary place, should look like this:

  Linux 32
  ```
  ./linux32/libsteam_api.so
  ./godot_binary
  ```
  
  Linux 64
  ```
  ./linux64/libsteam_api.so
  ./godot_binary
  ```
  
  Windows 32
  ```
  ./steam_api.dll
  ./godot_binary.exe
  ```
  
  Windows 64
  ```
  ./steam_api64.dll
  ./godot_binary.exe
  ```

API Reference
-------------
Steam
```
bool init()
	Needs to be called in order for this module to work properly. Returns true if succeed.

bool is_steam_running()
	Checks if a local Steam client is running.

void run_callbacks()
	This is required for many functions to work properly (f.e. for users to emit "avatar_loaded" signal).
	It should be called at least few times a second. Recommended to use it inside "_process".

SteamUser get_user()
	Returns current Steam user (client).

int get_appid()
	Returns game Steam AppID

String get_userdata_path()
	Returns steam userdata path ( something like "C:\Progam Files\Steam\userdata\<SteamID>\<AppID>\local" )

void set_game_info( SteamID server, String server_ip, int port ) 
	Updates info about server you are playing on.

Array get_friends( int filter=NOT_OFFLINE )
	Returns Array of `SteamUser` from your friends list. List is filtered by given filter, which can be:
	OFFLINE|ONLINE|BUSY|AWAY|SNOOZE|LF_TRADE|LF_PLAY|NOT_OFFLINE|ALL

Array get_groups()
	Returns Array of `SteamGroup`.

bool overlay_is_enabled()
	Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
	start & hook the game process, so this function will initially return false while the overlay is loading.

void overlay_set_notification_pos( int(0-3) pos )
	Change position(corner) at which notifications (messages, etc.) will be displayed.
	Constants: TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT.

void overlay_open( String type="" )
	Opens Steam overlay. Optional types are (not case-sensitive), not all tested:
	"Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements".

void overlay_open_user( String type="", SteamID user )
	Type "chat" can be opened for both `SteamUser` and `SteamGroup`. Valid types are:
		"steamid", "chat", "jointrade", "stats", "achievements", 
		"friendadd", "friendremove", "friendrequestaccept", "friendrequestignore"
	Barely tested.

void overlay_open_url( String url )
	Opens URL using Steam overlay web browser

void overlay_open_store( int appID=0 )
	Opens specified app store site. If invalid or none given, main Steam store site will open.
```
SteamID
```
bool is_valid()
	Returns true if it's valid (got proper ID assigned).

int get_steamID()
	Returns SteamID.

int get_accountID()
	Returns AccountID. Usually the same as SteamID.

int get_account_type()
	Returns account type associated to this SteamID. 
	Usually: TYPE_INDIVIDUAL|TYPE_GROUP|TYPE_CHAT|TYPE_GAMESERVER.

int get_universe()
	Returns associated universe. Probably always UNIVERSE_PUBLIC.

String get_profile_url()
	Returns string that can be used (using web browser) to open this user or group profile page.
	Only valid for TYPE_INDIVIDUAL|TYPE_GROUP.
```
SteamUser (extends SteamID)
`*` - means it works only for YOU, `**` - only for USER
```
String get_name()
	Returns user nickname.

int get_state()
	Returns: OFFLINE|ONLINE|BUSY|AWAY|SNOOZE|LF_TRADE|LF_PLAY

bool load_avatar(int size)
	Loads user avatar. After avatar gets loaded, signal "avatar_loaded", containing 2 arguments -
	size and avatar as `Image`, will be emitted.
	Returns false if avatar won't be loaded (probably only if: invalid size given or steam api not working)
	Valid sizes: AVATAR_SMALL|AVATAR_MEDIUM|AVATAR_LARGE
					(32x32)		(64x64)		 (184x184)

int user_get_steamlevel()
	Returns user Steam level. 
	It should work with USER but for me it is just returning -1.

int get_user_type()
	Returns: INVALID|YOU|USER

String get_rich_presence( String key )
* bool set_rich_presence( String key, String value )
	Rich Presence data is automatically shared between friends who are in the same game
	Each user has a set of Key/Value pairs
	Up to 20 different keys can be set
	There are two magic keys:
			"status"  - string that will show up in the 'view game info' dialog in the Steam friends list
			"connect" - string that contains the command-line for how a friend can connect to a game
	get_rich_presence() returns an empty string "" if no value is set
	set_rich_presence() using empty string "" deletes the key.

* bool clear_rich_presence()
	Deletes all your rich_presence data.

** bool is_friend()
	Checks if this user is on your friend list.

* bool is_logged() - probably will get moved to `Steam`
	Returns true if the Steam client current has a live connection to the Steam servers.
	If false, it means there is no active connection due to either a networking issue on the local machine, or the Steam server is down/busy.
	The Steam client will automatically be trying to recreate the connection as often as possible.
```
SteamGroup (SteamID)
```
String get_name()
	Returns group name.

bool has_tag()
	False if get_tag() returns empty string.

String get_tag()
	Returns group tag.

void open_chat()
	Opens group chat in the overlay.
```
License
-------------
MIT license
