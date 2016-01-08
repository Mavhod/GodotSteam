
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "reference.h"

#include "steamuser.h"

class GodotSteam: public Reference
{
public:
	GodotSteam();
	~GodotSteam();
	bool init();
	Ref<GSteamUser> get_user();

protected:
	static void _bind_methods();

private:
	bool isInitSuccess;
	Ref<GSteamUser> steamUser;
	OBJ_TYPE(GodotSteam, Reference);
};

#endif // GODOTSTEAM_H


