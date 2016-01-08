
#include "godotsteam.h"
#include <steam/steam_api.h>

GodotSteam::GodotSteam()
{
	isInitSuccess = false;
	steamUser = Ref<GSteamUser>(memnew(GSteamUser));
}

bool GodotSteam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();
	return isInitSuccess;
}

Ref<GSteamUser> GodotSteam::get_user()
{
	if (isInitSuccess && SteamUser() != steamUser->get_isteamuser())
	{
		steamUser->set_isteamuser( SteamUser() );
	}
	return steamUser;
}

void GodotSteam::_bind_methods()
{
    ObjectTypeDB::bind_method("init",&GodotSteam::init);
	ObjectTypeDB::bind_method("get_user",&GodotSteam::get_user);
}

GodotSteam::~GodotSteam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
}

