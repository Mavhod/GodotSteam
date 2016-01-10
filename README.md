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
	If success return true.

bool is_steam_running()
	Checks if a local Steam client is running.

int get_appid()
	Returns game Steam AppID

string get_userdata_path()
	Returns steam userdata path ( something like "C:\Progam Files\Steam\userdata\<SteamID>\<AppID>\local" )

bool user_is_logged()
	Returns true if the Steam client current has a live connection to the Steam servers.
	If false, it means there is no active connection due to either a networking issue on the local machine, or the Steam server is down/busy.
	The Steam client will automatically be trying to recreate the connection as often as possible.

int user_get_id()
	Returns Steam account ID.

string user_get_name()
	Returns user nickname.

int user_get_steam_level()
	Returns user Steam level.

string user_get_profile_link()
	Returns link to user's profile ( http://steamcommunity.com/profiles/[U:<universe>:<account_id>] )

void user_set_server_info( int server_SteamID, string server_ip, int port)
	Updates info about server the user is playing on.

bool overlay_is_enabled()
	Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
	start & hook the game process, so this function will initially return false while the overlay is loading.

void overlay_set_notification_pos( int(0-3) pos )
	Change position(corner) at which notifications (messages, etc.) will be displayed.
	Constants: TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT.

void overlay_open_url( string url )
	Opens URL using Steam overlay web browser
```
License
-------------
MIT license
