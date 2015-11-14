
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "reference.h"

class GodotSteam: public Reference
{
public:
	GodotSteam();
	~GodotSteam();
	bool init();

protected:
	static void _bind_methods();

private:
	bool isInitSuccess;
	OBJ_TYPE(GodotSteam, Reference);
};

#endif // GODOTSTEAM_H


