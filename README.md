# GodotSteam
Steam api for Godot2.

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
  
  OSX
  ```
  ./libsteam_api.dylib
  ./godot_binary
  ```

API Reference
-------------
```
bool init()
If success return true.


bool cloudIsEnabledForAccount()

bool cloudIsEnabledForApp()

uint32_t cloudGetCount()

Dictionary cloudGetFileNameAndSize( int iFile )
iFile: Index of file
Return dictionary members: fileName, fileSize

bool cloudFileWrite( const String &pchFile, const Variant &pvData )
pchFile: File name
pvData: Data for written

Variant cloudFileRead( const String &pchFile )
pchFile: File name
Return data read

bool cloudFileDelete( const String &pchFile )
pchFile: File name


uint32_t achievementsGetNum()

String achievementGetName( uint32_t iAchievement )

bool achievementSet( const String &pchName )

Dictionary achievementGet( const String &pchName )
Return dictionary members: isSuccessGet, isAchieved

bool achievementClear( const String &pchName )
```

License
-------------
MIT license
