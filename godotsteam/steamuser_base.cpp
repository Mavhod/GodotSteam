
#include "steamuser_base.h"


// Returns Steam3 ID of current user
int SteamID::get_steamID()
{
	return cSteamID.ConvertToUint64();
}

// Get user Account ID, usually it's the same as user Steam ID
int SteamID::get_accountID()
{
	return cSteamID.GetAccountID();
}

// Get user universe (check constants). Mostly will return 1 - UNIVERSE_PUBLIC
int SteamID::get_universe()
{
	return cSteamID.GetEUniverse();
}

String SteamID::get_rich_presence(const String& s_key)
{
	if ( SteamFriends() == NULL ) { return ""; }
	return SteamFriends()->GetFriendRichPresence( cSteamID, s_key.utf8().get_data() );
}

// Get link to user's SteamCommunity profile
String SteamID::get_profile_url()
{
	String link="http://steamcommunity.com/profiles/";
	String u_account_id = itos(get_accountID());
	String u_universe = itos(get_universe());
	// [ <AccountType> : <Universe> : <AccountID> ]
	// U - individual account
	link += "[U:"+u_universe+":"+u_account_id+"]";
	return link;
}

void SteamID::_bind_methods()
{
	ObjectTypeDB::bind_method("get_steamID",&SteamID::get_steamID);
	ObjectTypeDB::bind_method("get_accountID",&SteamID::get_accountID);
	ObjectTypeDB::bind_method("get_universe",&SteamID::get_universe);
	ObjectTypeDB::bind_method("get_name",&SteamID::get_name);
	ObjectTypeDB::bind_method("get_state",&SteamID::get_state);
	ObjectTypeDB::bind_method(_MD("get_rich_presence","key"),&SteamID::get_rich_presence);
	ObjectTypeDB::bind_method("get_profile_url",&SteamID::get_profile_url);
	//BIND_VMETHOD(MethodInfo(Variant::STRING,"get_name"));
	
	BIND_CONSTANT(OFFLINE); // 0
	BIND_CONSTANT(ONLINE); // 1
	BIND_CONSTANT(BUSY); // ...
	BIND_CONSTANT(AWAY);
	BIND_CONSTANT(SNOOZE);
	BIND_CONSTANT(LF_TRADE);
	BIND_CONSTANT(LF_PLAY);
//	BIND_CONSTANT(MAX);
//	BIND_CONSTANT(NOT_OFFLINE); // custom - 8
//	BIND_CONSTANT(ALL); // custom - 9
	
	BIND_CONSTANT(UNIVERSE_INVALID);
	BIND_CONSTANT(UNIVERSE_PUBLIC);
	BIND_CONSTANT(UNIVERSE_BETA);
	BIND_CONSTANT(UNIVERSE_INTERNAL);
	BIND_CONSTANT(UNIVERSE_DEV);
}
