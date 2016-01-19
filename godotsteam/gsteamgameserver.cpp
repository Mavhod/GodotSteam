
#include "gsteamgameserver.h"
#include <steam/steam_gameserver.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

_SteamGameServer* _SteamGameServer::singleton = NULL;

_SteamGameServer::_SteamGameServer()
{
	singleton = this;
}

_SteamGameServer* _SteamGameServer::get_singleton() { return singleton; }


bool _SteamGameServer::init(const String& ipAddress, int steamPort, int gamePort, int queryPort, int serverMode)
{
	// resolve address and convert it to int (IP_Address) union
	IP_Address addr(IP::get_singleton()->resolve_hostname(ipAddress));
	// for some reason bytes are in wrong order, need to swap them
	for(int i=0;i<2;i++)
	{
		uint8 temp = addr.field[i];
		addr.field[i] = addr.field[3-i];
		addr.field[3-i] = temp;
	}
//	printf("IP int: %ld",addr.host);
	isInitSuccess = SteamGameServer_Init( addr.host, (uint16)steamPort, (uint16)gamePort, (uint16)queryPort, EServerMode(serverMode), serverVersion);
	return isInitSuccess;
	// SteamUser()->AdvertiseGame(gameserver->getCSteamID(), addr.host, port); 
}

void _SteamGameServer::logon( const String& token )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->LogOn( token.utf8().get_data() );
}

void _SteamGameServer::logon_anon()
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->LogOnAnonymous();
}

void _SteamGameServer::logoff()
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->LogOff();
}


bool _SteamGameServer::is_logged()
{
	if ( SteamGameServer() == NULL ) { return 0; }
	return SteamGameServer()->BLoggedOn();
}

String _SteamGameServer::get_ip()
{
	if ( SteamGameServer() == NULL ) { return ""; }
	IP_Address addr;
	addr.host = SteamGameServer()->GetPublicIP();
	String strAddress = itos(addr.field[3])+"."+itos(addr.field[2])+"."+itos(addr.field[1])+"."+itos(addr.field[0]);
	return strAddress;
}

Ref<SteamID> _SteamGameServer::get_steam_id()
{
	if ( SteamGameServer() != NULL && serverSteamID == NULL )
	{
		serverSteamID = Ref<SteamID>( memnew( SteamID(SteamGameServer()->GetSteamID()) ) );
	}
	return serverSteamID;
}


void _SteamGameServer::set_product( const String& pName )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->SetProduct( pName.utf8().get_data() );
}

void _SteamGameServer::set_description( const String& sDesc )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->SetGameDescription( sDesc.utf8().get_data() );
}

void _SteamGameServer::enable_heartbeats( bool active )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->EnableHeartbeats( active );
}

void _SteamGameServer::set_server_name( const String& sName )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->SetServerName( sName.utf8().get_data() );
}



void _SteamGameServer::set_max_players( int pCount )
{
	if ( SteamGameServer() == NULL ) { return; }
	SteamGameServer()->SetMaxPlayerCount( pCount );
}







void _SteamGameServer::_bind_methods()
{
	ObjectTypeDB::bind_method(_MD("test_init","IP","steam_port","game_port","query_port","server_mode"),&_SteamGameServer::init);
	ObjectTypeDB::bind_method(_MD("test_logon","token"),&_SteamGameServer::logon);
	ObjectTypeDB::bind_method(_MD("test_logon_anon","token"),&_SteamGameServer::logon_anon);
	ObjectTypeDB::bind_method("is_logged",&_SteamGameServer::is_logged);
	ObjectTypeDB::bind_method("get_ip",&_SteamGameServer::get_ip);
	ObjectTypeDB::bind_method("get_steam_id",&_SteamGameServer::get_steam_id);
	
	ObjectTypeDB::bind_method(_MD("set_product","name"),&_SteamGameServer::set_product);
	ObjectTypeDB::bind_method(_MD("set_description","desc"),&_SteamGameServer::set_description);
	ObjectTypeDB::bind_method(_MD("enable_hbeats","active"),&_SteamGameServer::enable_heartbeats);
	ObjectTypeDB::bind_method(_MD("set_server_name","name"),&_SteamGameServer::set_server_name);
	ObjectTypeDB::bind_method(_MD("set_max_players","count"),&_SteamGameServer::set_max_players);
	// BIND_CONSTANT(INVALID);
}

_SteamGameServer::~_SteamGameServer()
{
	if(isInitSuccess)
	{
		SteamGameServer_Shutdown();
	}
	singleton = NULL;
}