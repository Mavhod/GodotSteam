
#include "gsteamuser.h"

_SteamUser::_SteamUser(CSteamID cSID)
{
	setCSteamID( cSID );
	// try to find its type
}

bool _SteamUser::updateType()
{
	int old_type = get_user_type();
	setUserType(INVALID);
	CSteamID your_id = getCSteamID();
	if ( SteamUser() != NULL && your_id == SteamUser()->GetSteamID() )
	{ 
		setUserType(YOU);
	}
	else if ( SteamFriends() != NULL )
	{
		if ( SteamFriends()->HasFriend(your_id,0x04) == true ) // "regular" friend
		{ setUserType(USER); }
		else if ( SteamFriends()->HasFriend(your_id,0x00) == false )
		{ setUserType(USER); }
	}
	return (old_type != get_user_type());
}


// sets to INVALID if unknown type
void _SteamUser::setUserType(int uType)
{
	if ( uType<0 || uType>USER_TYPE_MAX )
		uType = INVALID;
	user_type = uType;
}

String _SteamUser::get_name()
{
	if ( SteamFriends() == NULL ) { return ""; }
	if ( get_user_type() == YOU )
		return SteamFriends()->GetPersonaName();
	else if ( get_user_type() == USER )
		return SteamFriends()->GetFriendPersonaName( getCSteamID() );
	return "";
}

int _SteamUser::get_state()
{
	if ( SteamFriends() == NULL ) { return 0; }
	if ( get_user_type() == YOU )
		return SteamFriends()->GetPersonaState();
	else if ( get_user_type() == USER )
		return SteamFriends()->GetFriendPersonaState( getCSteamID() );
	return 0;
}

int _SteamUser::get_steamlevel()
{
	if ( SteamUser() == NULL ) { return 0; }
	if ( get_user_type() == YOU )
	{ return SteamUser()->GetPlayerSteamLevel(); }
	else if ( get_user_type() == USER )
	{ return SteamFriends()->GetFriendSteamLevel( getCSteamID() ); }
	return 0;
}

bool _SteamUser::is_logged()
{
	if ( SteamUser() == NULL ) { return false; }
	if ( get_user_type() == YOU )
	{
		return SteamUser()->BLoggedOn();
	}
	return false;
}

bool _SteamUser::set_rich_presence(const String& s_key, const String& s_value)
{
	if ( SteamFriends() == NULL ) { return false; }
	if ( get_user_type() == YOU )
	{
		return SteamFriends()->SetRichPresence(s_key.utf8().get_data(),s_value.utf8().get_data());
	}
	return false;
}

String _SteamUser::get_rich_presence(const String& s_key)
{
	if ( SteamFriends() == NULL ) { return ""; }
	if ( get_user_type() == YOU || get_user_type() == USER )
	{
		return SteamFriends()->GetFriendRichPresence(getCSteamID(), s_key.utf8().get_data());
	}
	return "";
}

// Cleans up all the keys
bool _SteamUser::clear_rich_presence()
{
	if ( SteamFriends() == NULL ) { return false; }
	if ( get_user_type() == YOU )
	{
		SteamFriends()->ClearRichPresence();
		return true;
	}
	return false;	
}

// Checks if user is on your friends list
bool _SteamUser::is_friend()
{
	if ( SteamFriends() == NULL ) { return false; }
	if ( get_user_type() == USER )
	{
		return SteamFriends()->HasFriend(getCSteamID(),0x04);
	}
	return false;	
}

int _SteamUser::get_relationship()
{
	if ( SteamFriends() == NULL ) { return REL_NONE; }
	if ( get_user_type() == USER )
	{
		return SteamFriends()->GetFriendRelationship( getCSteamID() );
	}
	return REL_NONE;
}

void _SteamUser::_bind_methods()
{
	ObjectTypeDB::bind_method("is_logged",&_SteamUser::is_logged);
	ObjectTypeDB::bind_method("is_friend",&_SteamUser::is_friend);
	ObjectTypeDB::bind_method("get_user_type",&_SteamUser::get_user_type);
	ObjectTypeDB::bind_method("get_name",&_SteamUser::get_name);
	ObjectTypeDB::bind_method("get_state",&_SteamUser::get_state);
	ObjectTypeDB::bind_method("get_steamlevel",&_SteamUser::get_steamlevel);
	ObjectTypeDB::bind_method(_MD("get_rich_presence","key"),&_SteamUser::get_rich_presence);
	ObjectTypeDB::bind_method(_MD("set_rich_presence","key","value"),&_SteamUser::set_rich_presence);
	ObjectTypeDB::bind_method(_MD("clear_rich_presence"),&_SteamUser::clear_rich_presence);
	// ObjectTypeDB::bind_method(_MD("get_relationship"),&_SteamUser::get_relationship);
	
	BIND_CONSTANT(INVALID);
	BIND_CONSTANT(YOU);
	BIND_CONSTANT(USER);
	
	BIND_CONSTANT(OFFLINE);
	BIND_CONSTANT(ONLINE);
	BIND_CONSTANT(BUSY);
	BIND_CONSTANT(AWAY);
	BIND_CONSTANT(SNOOZE);
	BIND_CONSTANT(LF_TRADE);
	BIND_CONSTANT(LF_PLAY);
}

