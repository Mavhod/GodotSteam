
#include "gsteamid.h"


bool SteamID::is_valid()
{
	return cSteamID.IsValid();
}

// Returns Steam3 ID (uint64)
int SteamID::get_steamID()
{
	return cSteamID.ConvertToUint64();
}

// Get account ID, usually it's the same as user Steam ID
int SteamID::get_accountID()
{
	return cSteamID.GetAccountID();
}

// Get universe (check constants). Mostly will return 1 - UNIVERSE_PUBLIC
int SteamID::get_universe()
{
	return cSteamID.GetEUniverse();
}

int SteamID::get_account_type()
{
	return cSteamID.GetEAccountType();
}

// Get link to user's SteamCommunity profile
String SteamID::get_profile_url()
{
	String link="http://steamcommunity.com/";
	int id_type = get_account_type();
	if (id_type == TYPE_INDIVIDUAL || id_type == TYPE_GROUP)
	{
		// [ <AccountType> : <Universe> : <AccountID> ]
		if (id_type == TYPE_INDIVIDUAL) 
			link += "profiles/[U:";
		else // group
			link += "gid/[g:";
		String accid = itos(get_accountID());
		String universe = itos(get_universe());
		link += universe+":"+accid+"]";
	}
	return link;
}

void SteamID::_bind_methods()
{
	// ObjectTypeDB::bind_method("get_universe",&SteamID::get_universe);
	// ObjectTypeDB::bind_method("get_name",&SteamID::get_name);
	// ObjectTypeDB::bind_method("get_state",&SteamID::get_state);
	//ObjectTypeDB::bind_method(_MD("get_rich_presence","key"),&SteamID::get_rich_presence);
	
	ObjectTypeDB::bind_method("is_valid",&SteamID::is_valid);
	ObjectTypeDB::bind_method("get_id",&SteamID::get_steamID);
	ObjectTypeDB::bind_method("get_account_id",&SteamID::get_accountID);
	ObjectTypeDB::bind_method("get_account_type",&SteamID::get_account_type);
	ObjectTypeDB::bind_method("get_universe",&SteamID::get_universe);
	
	ObjectTypeDB::bind_method("get_profile_url",&SteamID::get_profile_url);
	//BIND_VMETHOD(MethodInfo(Variant::STRING,"get_name"));
	
	// ADD_SIGNAL(MethodInfo("steamid_changed"));
	
	BIND_CONSTANT(UNIVERSE_INVALID);
	BIND_CONSTANT(UNIVERSE_PUBLIC);
	BIND_CONSTANT(UNIVERSE_BETA);
	BIND_CONSTANT(UNIVERSE_INTERNAL);
	BIND_CONSTANT(UNIVERSE_DEV);
	
	BIND_CONSTANT(TYPE_INVALID);
	BIND_CONSTANT(TYPE_INDIVIDUAL);
	BIND_CONSTANT(TYPE_MULTISEAT);
	BIND_CONSTANT(TYPE_GAMESERVER);
	BIND_CONSTANT(TYPE_ANON_GAMESERVER);
	BIND_CONSTANT(TYPE_PENDING);
	BIND_CONSTANT(TYPE_CONTENTSERVER);
	BIND_CONSTANT(TYPE_GROUP);
	BIND_CONSTANT(TYPE_CHAT);
	BIND_CONSTANT(TYPE_CONSOLEUSER);
	BIND_CONSTANT(TYPE_ANON_USER);
}
