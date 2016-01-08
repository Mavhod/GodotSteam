
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
	int get_steam_level();
	void advertise_game(int host_id, const String& server_ip, int port);
	
	void set_isteamuser( ISteamUser* iSteamUser );
	ISteamUser* get_isteamuser();
	
protected:
	static void _bind_methods();
	
private:
	OBJ_TYPE(GSteamUser, Reference);
	ISteamUser* iSteamUser;
	bool isEmpty;
};