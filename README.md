# GodotSteam
Steam api for Godot game engine.

For Windows and Linux platform.

APIs for steam I will add it later.

For Windows I quite not test yet, maybe it has a tiny issue.

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

  For Linux we must add ```openssl=no``` when compile because it has problem with lib crypto (class StreamPeerSSL may can't use)

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

GSteamUser get_user()
Returns current user.
```
GSteamUser
```
int get_id()
Returns ID representated by 64bit unsigned int (Steam3 ID).

bool is_logged()
Returns true if the Steam client current has a live connection to the Steam servers.
If false, it means there is no active connection due to either a networking issue on the local machine, or the Steam server is down/busy.
The Steam client will automatically be trying to recreate the connection as often as possible.

bool is_empty()
Checks if GSteamUser contains any data.
```
License
-------------
MIT license
