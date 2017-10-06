
#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include "reference.h"
// #include "dictionary.h"


class GodotSteam: public Reference
{
public:
	GodotSteam();
	~GodotSteam();
	bool init();
	//
	bool cloudIsEnabledForAccount();
	bool cloudIsEnabledForApp();
	uint32_t cloudGetCount();
	Dictionary cloudGetFileNameAndSize( int iFile );
	bool cloudFileWrite( const String &pchFile, const Variant &pvData );
	Variant cloudFileRead( const String &pchFile );
	bool cloudFileDelete( const String &pchFile );
	//
	bool statSet( const String &pchName, const Variant &data );
	uint32_t achievementsGetNum();
	String achievementGetName( uint32_t iAchievement );
	bool achievementSet( const String &pchName );
	Dictionary achievementGet( const String &pchName );
	bool achievementClear( const String &pchName );

protected:
	static void _bind_methods();

private:
	bool isInitSuccess;
	OBJ_TYPE(GodotSteam, Reference);
};

#endif // GODOTSTEAM_H


