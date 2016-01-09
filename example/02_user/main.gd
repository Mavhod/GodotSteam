extends Node



func _ready():
	var steam = GodotSteam.new()
	steam.init()
	print("Steam3 ID: ",steam.user_get_id())
	print("Your nickname: ",steam.user_get_name())
	print("Your steam level: ",steam.user_get_steam_level())
	# After using function below, if someone clicks "Join game" on you, this server IP and port will be displayed
	# First argument should be Server SteamID, I just used our SteamID for testing purpose
	#
	# user_set_server_info( int server_steamID, string server_ip, int port )
	steam.user_set_server_info(steam.user_get_id(),"127.0.0.1",11234)


