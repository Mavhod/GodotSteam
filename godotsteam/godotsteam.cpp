
#include "godotsteam.h"
#include <steam/steam_api.h>
#include "io/marshalls.h"
#include "typedefs.h"
//#include "variant.h"

GodotSteam::GodotSteam()
{
	isInitSuccess = false;
}

bool GodotSteam::init()
{
	//printf("Godot steam init\n");
	isInitSuccess = SteamAPI_Init();

	if(!isInitSuccess)
	{
		return false;
	}

	SteamUserStats()->RequestCurrentStats();
	
	return true;
}

bool GodotSteam::cloudIsEnabledForAccount()
{
	return SteamRemoteStorage()->IsCloudEnabledForAccount();
}

bool GodotSteam::cloudIsEnabledForApp()
{
	return SteamRemoteStorage()->IsCloudEnabledForApp();
}

uint32_t GodotSteam::cloudGetCount()
{
	return SteamRemoteStorage()->GetFileCount();
}

Dictionary GodotSteam::cloudGetFileNameAndSize( int iFile )
{
	int32_t fileSize = 0;
	const char *fileName = SteamRemoteStorage()->GetFileNameAndSize( iFile, &fileSize );

	Dictionary file;
	file["fileName"] = fileName;
	file["fileSize"] = fileSize;

	return file;
}

bool GodotSteam::cloudFileWrite( const String &pchFile, const Variant &pvData )
{
	//printf("%s\n", pchFile.utf8().get_data());
	
	int len;
	Error err = encode_variant(pvData, NULL, len);
	if(err != OK)
	{
		//printf("GodotSteam::cloudFileWrite: encode_variant(pvData, NULL, len) failed\n");
		return false;
	}

	DVector<uint8_t> buff;
	buff.resize(len);
	DVector<uint8_t>::Write w = buff.write();

	err = encode_variant(pvData, &w[0], len);
	if(err != OK)
	{
		//printf("GodotSteam::cloudFileWrite: encode_variant(pvData, &w[0], len) failed\n");
		return false;
	}
	w = DVector<uint8_t>::Write();

	DVector<uint8_t>::Read r = buff.read();
	bool bRet = SteamRemoteStorage()->FileWrite( pchFile.utf8().get_data(), &r[0], len );
//	bool bRet = SteamRemoteStorage()->FileWrite( "save/data00.dat", "sss", 4 );

	return bRet;
}

Variant GodotSteam::cloudFileRead( const String &pchFile )
{
	Variant v;
	
	if ( !SteamRemoteStorage()->FileExists( pchFile.utf8().get_data() ) )
	{
		printf("GodotSteam::cloudFileRead: File not Exists\n");
		return v;
	}

	// len
	int32_t len = SteamRemoteStorage()->GetFileSize( pchFile.utf8().get_data() );
	if(len <= 0)
	{
		printf("GodotSteam::cloudFileRead: File size <= 0\n");
		return v;
	}

	// v
	DVector<uint8_t> buff;
	buff.resize(len);
	DVector<uint8_t>::Read r = buff.read();
	int32_t cubRead = SteamRemoteStorage()->FileRead( pchFile.utf8().get_data(), (void*)&r[0], len );
	Error err = decode_variant(v, &r[0], len);
	if(err != OK)
	{
		printf("GodotSteam::cloudFileRead: decode_variant failed\n");
		return v;
	}

	return v;
}

bool GodotSteam::cloudFileDelete( const String &pchFile )
{
	return SteamRemoteStorage()->FileDelete( pchFile.utf8().get_data() );
}

bool GodotSteam::statSet( const String &pchName, const Variant &data )
{
	bool ret = false;
	
	if(data.get_type() == Variant::INT)
	{
		int32_t i = data;
		ret = SteamUserStats()->SetStat( pchName.utf8().get_data(), i );
	}
	else if(data.get_type() == Variant::REAL)
	{
		float f = data;
		ret = SteamUserStats()->SetStat( pchName.utf8().get_data(), f );
	}
	
	if(!ret)
	{
		return false;
	}

	return SteamUserStats()->StoreStats();
}

uint32_t GodotSteam::achievementsGetNum()
{
	return SteamUserStats()->GetNumAchievements(); 
}

String GodotSteam::achievementGetName( uint32_t iAchievement )
{
	return SteamUserStats()->GetAchievementName(iAchievement);
}

bool GodotSteam::achievementSet( const String &pchName )
{
	if( !SteamUserStats()->SetAchievement( pchName.utf8().get_data() ) )
	{
		return false;
	}

	return SteamUserStats()->StoreStats();
}

Dictionary GodotSteam::achievementGet( const String &pchName )
{
	bool pbAchieved = false;
	bool ret = SteamUserStats()->GetAchievement( pchName.utf8().get_data(), &pbAchieved );

	Dictionary status;
	status["isAchieved"] = pbAchieved;
	status["isSuccessGet"] = ret;

	return status;
}

bool GodotSteam::achievementClear( const String &pchName )
{
	if( !SteamUserStats()->ClearAchievement( pchName.utf8().get_data() ) )
	{
		return false;
	}

	return SteamUserStats()->StoreStats();
}

void GodotSteam::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("init"), &GodotSteam::init);
    ObjectTypeDB::bind_method(_MD("cloudIsEnabledForAccount"), &GodotSteam::cloudIsEnabledForAccount);
    ObjectTypeDB::bind_method(_MD("cloudIsEnabledForApp"), &GodotSteam::cloudIsEnabledForApp);
    ObjectTypeDB::bind_method(_MD("cloudGetCount"), &GodotSteam::cloudGetCount);
    ObjectTypeDB::bind_method(_MD("cloudGetFileNameAndSize", "iFile"), &GodotSteam::cloudGetFileNameAndSize);
    ObjectTypeDB::bind_method(_MD("cloudFileWrite", "pchFile", "pvData"), &GodotSteam::cloudFileWrite);
    ObjectTypeDB::bind_method(_MD("cloudFileRead", "pchFile"), &GodotSteam::cloudFileRead);
    ObjectTypeDB::bind_method(_MD("cloudFileDelete", "pchFile"), &GodotSteam::cloudFileDelete);
    ObjectTypeDB::bind_method(_MD("statSet", "pchName", "data"), &GodotSteam::statSet);
    ObjectTypeDB::bind_method(_MD("achievementsGetNum"), &GodotSteam::achievementsGetNum);
    ObjectTypeDB::bind_method(_MD("achievementGetName", "iAchievement"), &GodotSteam::achievementGetName);
    ObjectTypeDB::bind_method(_MD("achievementSet", "pchName"), &GodotSteam::achievementSet);
    ObjectTypeDB::bind_method(_MD("achievementGet", "pchName"), &GodotSteam::achievementGet);
    ObjectTypeDB::bind_method(_MD("achievementClear", "pchName"), &GodotSteam::achievementClear);
}

GodotSteam::~GodotSteam()
{
	if(isInitSuccess)
	{
		//printf("Godot steam exit\n");
		SteamAPI_Shutdown();
	}
}

