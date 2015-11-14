
#include "godotsteam.h"
#include <steam/steam_api.h>

GodotSteam::GodotSteam()
{
	isInitSuccess = false;
}

bool GodotSteam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();
	return isInitSuccess;
}

void GodotSteam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&GodotSteam::init);
}

GodotSteam::~GodotSteam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
}

