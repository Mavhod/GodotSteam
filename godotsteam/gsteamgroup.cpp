
#include "gsteamgroup.h"
#include "godotsteam.h"

_SteamGroup::_SteamGroup(CSteamID cSID)
{
	setCSteamID( cSID );
	// _steamid_changed() called
}


String _SteamGroup::get_name()
{
	if ( SteamFriends() == NULL ) { return ""; }
	return SteamFriends()->GetClanName( getCSteamID() );
}

String _SteamGroup::get_tag()
{
	if ( SteamFriends() == NULL ) { return ""; }
	return SteamFriends()->GetClanTag( getCSteamID() );
}

bool _SteamGroup::has_tag()
{
	if ( SteamFriends() == NULL ) { return false; }
	return (get_tag() != "");
}

void _SteamGroup::open_chat()
{
	if ( SteamFriends() == NULL ) { return; }
	// SteamFriends()->OpenClanChatWindowInSteam( getCSteamID() );
	// ^ This doesn't work for some reason
	// Use Steam.overlay function then
	Steam->get_singleton().overlay_open_user("chat",getCSteamID());
}


void _SteamGroup::_bind_methods()
{
	ObjectTypeDB::bind_method("get_name",&_SteamGroup::get_name);
	ObjectTypeDB::bind_method("has_tag",&_SteamGroup::has_tag);
	ObjectTypeDB::bind_method("get_tag",&_SteamGroup::get_tag);
	ObjectTypeDB::bind_method("open_chat",&_SteamGroup::open_chat);
	// BIND_CONSTANT(INVALID);
}

