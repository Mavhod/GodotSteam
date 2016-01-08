
#include "steamuser.h"

GSteamUser::GSteamUser()
{
	isEmpty = true;
}

GSteamUser::GSteamUser( ISteamUser* iSteamUser )
{
	this->iSteamUser = iSteamUser;
	isEmpty = false;
}


// Returns Steam3 ID of current user
int GSteamUser::get_id()
{
	if (isEmpty) { return 0; }
	CSteamID cSteamID = iSteamUser->GetSteamID();
	return cSteamID.ConvertToUint64();
//	return 0;
}

// Checks if Steam is running properly (got connection to Steam servers)
bool GSteamUser::is_logged()
{
	if (isEmpty) { return false; }
	return iSteamUser->BLoggedOn();
//	return false;
}

// Does it have ISteamUser assigned?
bool GSteamUser::is_empty() { return isEmpty; }

// Sets ISteamUser (check steam/isteamuser.h)
void GSteamUser::set_isteamuser( ISteamUser* iSteamUser )
{
	this->iSteamUser = iSteamUser;
	isEmpty = false;
}

// Gets assigned ISteamUser
ISteamUser* GSteamUser::get_isteamuser() { return iSteamUser; }

void GSteamUser::_bind_methods()
{
	ObjectTypeDB::bind_method("get_id",&GSteamUser::get_id);
	ObjectTypeDB::bind_method("is_logged",&GSteamUser::is_logged);
	ObjectTypeDB::bind_method("is_empty",&GSteamUser::is_empty);
}

GSteamUser::~GSteamUser()
{
	//free(iSteamUser);
}