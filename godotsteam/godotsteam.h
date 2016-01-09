
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "reference.h"

class GodotSteam: public Reference
{
public:
	GodotSteam();
	~GodotSteam();
	
	bool init();
	bool is_steam_running();
	
	bool user_is_logged();
	int user_get_id();
	String user_get_name();
	int user_get_steam_level();
	
//	void set_username(const String& new_name);
	
	void set_server_info(int host_steamid, const String& server_ip, int port);

protected:
	static void _bind_methods();

private:
	bool isInitSuccess;
	OBJ_TYPE(GodotSteam, Reference);
};
#endif // GODOTSTEAM_H


