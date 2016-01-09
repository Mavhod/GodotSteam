# GodotSteam
Steam api for Godot game engine.

For Windows and Linux platform.

APIs for steam I will add it later.

How to use
----------
- Download Steamworks SDK.

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
  
  For Windows you can only compile with Visual C++ can't use MINGW because *.lib is exclusive for Microsoft compiler.

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
GodotSteam
```
bool init()
If success return true.

bool is_steam_running()
Checks if a local Steam client is running.

bool user_is_logged()
Returns true if the Steam client current has a live connection to the Steam servers.
If false, it means there is no active connection due to either a networking issue on the local machine, or the Steam server is down/busy.
The Steam client will automatically be trying to recreate the connection as often as possible.

int user_get_id()
Returns ID representated by 64bit unsigned int (Steam3 ID).

string user_get_name()
Returns user nickname.

int user_get_steam_level()
Returns user Steam level.

void user_set_server_info( int server_SteamID, string server_ip, int port)
Updates info about server the user is playing on.
```
License
-------------
MIT license
