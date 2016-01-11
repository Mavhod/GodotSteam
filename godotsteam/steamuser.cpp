
#include "steamuser.h"


String _SteamUser::get_name()
{
	if ( SteamFriends() == NULL ) { return ""; }
	return SteamFriends()->GetPersonaName();
}

int _SteamUser::get_state()
{
	if ( SteamFriends() == NULL ) { return 0; }
	return SteamFriends()->GetPersonaState();
}

int _SteamUser::get_steamlevel()
{
	if ( SteamUser() == NULL ) { return 0; }
	return SteamUser()->GetPlayerSteamLevel();
}

bool _SteamUser::is_logged()
{
	if ( SteamUser() == NULL ) { return false; }
	return SteamUser()->BLoggedOn();
}

bool _SteamUser::set_rich_presence(const String& s_key, const String& s_value)
{
	if ( SteamFriends() == NULL ) { return false; }
	return SteamFriends()->SetRichPresence(s_key.utf8().get_data(),s_value.utf8().get_data());
}

// Cleans up all the keys
void _SteamUser::clear_rich_presence()
{
	if ( SteamFriends() == NULL ) { return; }
	SteamFriends()->ClearRichPresence();
}

void _SteamUser::_bind_methods()
{
	ObjectTypeDB::bind_method("is_logged",&_SteamUser::is_logged);
	ObjectTypeDB::bind_method(_MD("set_rich_presence","key","value"),&_SteamUser::set_rich_presence);
	ObjectTypeDB::bind_method(_MD("clear_rich_presence"),&_SteamUser::clear_rich_presence);
}