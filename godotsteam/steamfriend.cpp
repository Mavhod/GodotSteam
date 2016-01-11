
#include "steamfriend.h"


String SteamFriend::get_name()
{
	if ( SteamFriends() == NULL ) { return ""; }
	return SteamFriends()->GetFriendPersonaName(cSteamID);
}

int SteamFriend::get_state()
{
	if ( SteamFriends() == NULL ) { return 0; }
	return SteamFriends()->GetFriendPersonaState(cSteamID);
}

int SteamFriend::get_steamlevel()
{
	if ( SteamFriends() == NULL ) { return 0; }
	return SteamFriends()->GetFriendSteamLevel( cSteamID );
}

// Checks if player associated to this ID is your friend
bool SteamFriend::is_friend()
{
	if (SteamFriends() == NULL) { return false; }
	return SteamFriends()->HasFriend(cSteamID, 0x04); // "regular" friend 0x04 (see EFriendFlags)
}
int SteamFriend::get_relationship()
{
	if (SteamFriends() == NULL) { return false; }
	return SteamFriends()->GetFriendRelationship( cSteamID );
}

void SteamFriend::_bind_methods()
{
	ObjectTypeDB::bind_method("is_friend",&SteamFriend::is_friend);
	ObjectTypeDB::bind_method("get_relationship",&SteamFriend::get_relationship);
}