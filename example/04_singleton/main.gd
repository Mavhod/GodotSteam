extends Node


func _ready():
	Steam.init()
	print("Userdata path: ",Steam.get_userdata_path())
	print("Steam ID: ",Steam.user_get_id())
	print("Your nickname: ",Steam.user_get_name())
	print("Your steam level: ",Steam.user_get_steam_level())
	# user_set_server_info( int server_steamID, string server_ip, int port )
	Steam.user_set_server_info(Steam.user_get_id(),"127.0.0.1",11234)
	# After using above function, if someone clicks "Join game" or "View game information" on you, this server IP and port will be displayed
	# First argument should be Server SteamID, I just used our user SteamID for testing purpose
	
	Steam.overlay_set_notification_pos( Steam.TOP_LEFT ) # display notifications in upper-left corner
	var url = Steam.user_get_profile_link()
	# Initializing Steam overlay may take a second or few, so this one probably won't work
	steam_open_url(url)
	# Lets make delayed url open then
	var timer = Timer.new()
	timer.set_wait_time(2) # 2 seconds
	timer.connect("timeout",self,"steam_open_url",[url])
	timer.connect("timeout",timer,"queue_free") # cleanup - remove timer on timeout
	add_child(timer)
	timer.start()

func steam_open_url( url ):
	if !Steam.overlay_is_enabled():
		print("Steam overlay isn't ready (yet?)")
	else:
		print("Opening url: ",url)
	# sometimes even if overlay_is_enabled == false overlay may open
	# f.e. change timer wait_time to 0.5 second and overlay should open while displaying "overlay isn't ready" message
	Steam.overlay_open_url(url)