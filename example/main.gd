extends Node

func _init():
	Steam.init()

func _ready():
	var t = get_node("Text")
	t.add_text(str("AppID: ",Steam.get_appid(),"\n"))
	t.add_text(str("Userdata path: ",Steam.get_userdata_path(),"\n"))
	t.add_text("\n\n")
	# display info about our own user
	var user = Steam.get_user()
	print_user( user )
	# load friends
	var friendlist = Steam.get_friends( Steam.NOT_OFFLINE ) # default filter
	print("Friends count: ",friendlist.size())
	for friend in friendlist:
		print_user( friend )
	# load groups
	var grouplist = Steam.get_groups()
	print("Groups count: ",grouplist.size())
	t.add_text("\nYOUR GROUPS (total:"+str(grouplist.size())+"): \n")
	for group in grouplist:
		# gonna display only those with tags, for less spam
		if group.has_tag():
			var gname = group.get_name()+" [ "+group.get_tag()+" ]"
			t.add_text("  "+gname+"\n")
	
	Steam.set_game_info(Steam.get_user(),"127.0.0.1",11234)
	# After using above function, if someone clicks "Join game" or "View game information" on you, this server IP and port will be displayed
	# First argument should be the Server object
	
	# check api reference for those
	user.set_rich_presence("status","Testing Steam API for Godot Engine")
	var launch_cmd = "-connect \"127.0.0.1\":11234" # or something like that
	user.set_rich_presence("connect",launch_cmd)
	user.set_rich_presence("custom_key","custom_value")
	# ---
	
	Steam.overlay_set_notification_pos( Steam.TOP_LEFT ) # display notifications in upper-left corner
	var url = user.get_profile_url()
	# Initializing Steam overlay may take a second or few, so this one probably won't work
	steam_open_url(url)
	# Wait 120 frames (~2sec) and try again
	for i in range(120):
		yield(get_tree(),"idle_frame")
	steam_open_url(url)

func print_user( user ):
	var t = get_node("Text")
	if !user extends SteamUser:
		t.add_text(str(user))
		t.add_text("Not a user!\n\n")
		return
	var type = user.get_user_type()
	var name = user.get_name()
	var state = user.get_state()
	# steamlevel might not work for other users for unknown reason
	var steamlevel = user.get_steamlevel()
	var steam_id = user.get_steamID()
	var acc_id = user.get_accountID()
	var is_friend = user.is_friend()
	
	var utypes = ["INVALID","YOU","USER"]
	var ustates = ["OFFLINE","ONLINE","BUSY","AWAY","SNOOZE","LF_TRADE","LF_PLAY"]
	
	
	t.add_text(str(utypes[type]))
	if is_friend:
		t.add_text(" (FRIEND)")
	t.add_text("\n")
	t.add_text(str("  Name: ",name,"\n"))
	t.add_text(str("  Status: ",ustates[state],"\n"))
	t.add_text(str("  Steam level: ",steamlevel,"\n"))
	t.add_text(str("  SteamID: ",steam_id,"\n"))
	t.add_text(str("  AccountID: ",acc_id,"\n"))
	t.add_text("\n")
#	t.add_text(str("","\n"))

func steam_open_url( url ):
	if !Steam.overlay_is_enabled():
		print("Steam overlay isn't ready (yet?)")
	else:
		print("Opening url: ",url)
	Steam.overlay_open_url(url)