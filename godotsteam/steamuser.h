
#include "reference.h"

#include <steam/isteamuser.h>

class GSteamUser: public Reference
{
public:
	GSteamUser();
	GSteamUser( ISteamUser* iSteamUser );
	~GSteamUser();
	int get_id();
	bool is_logged();
	bool is_empty();	
	
	void set_isteamuser( ISteamUser* iSteamUser );
	ISteamUser* get_isteamuser();
	
protected:
	static void _bind_methods();
	
private:
	OBJ_TYPE(GSteamUser, Reference);
	ISteamUser* iSteamUser;
	bool isEmpty;
};