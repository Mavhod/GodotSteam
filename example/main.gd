extends Node













func _ready():
	get_node("User").hide()
	
	INITIALIZING_STEAM()
	LOADING_USERS_DATA()
#	GROUPS() # not fully done, but you can check it
	OVERLAY()
	GAME_INFO()




























# ==========================================================================================
#  STEAM INITIALIZATION
# ==========================================================================================
func INITIALIZING_STEAM():
	# initialize SteamAPI
	var err = Steam.init()
	# Steam.init() can return:
	#  OK (0) - everything initialized properly, everything should work
	#  FAILED (1) - failed to initialize SteamAPI, nothing will work
	#  Steam.ERR_NO_CLIENT (2) - Steam client is not running, probably nothing will work
	#  Steam.ERR_NO_CONNECTION (3) - no connection to Steam servers, everything should work fine after you regain connection to Steam
	if err == FAILED or err == Steam.ERR_NO_CLIENT:
		print("Failed to initialize Steam. Shutting down...")
		get_tree().quit()
	# print some info
	print("App ID: ",Steam.get_appid())
	print("Steam userdata path: ",Steam.get_userdata_path())
	print("Username: ",Steam.get_user().get_name())
	# connect signals we might want to use later
	# "connection_changed" is triggered when you lose or regain connection to Steam servers
	Steam.connect("connection_changed",self,"_on_connection_changed")
	Steam.connect("overlay_toggled",self,"_on_overlay_toggled") # no description needed
	# make our signals (and other stuff) work!
	set_process(true) # run callbacks

func _on_connection_changed( connected ):
	if connected:
		print("Connected to Steam servers!")
	else: # real-time services disabled
		print("Disconnected from Steam servers :(")

func _on_overlay_toggled( active ):
	if active:
		print("Overlay ON")
	else:
		print("Overlay OFF")

# This processing is required for a lot of functions.
# Probably none of the signals will be emitted without it
func _process(delta):
	Steam.run_callbacks()








# ==========================================================================================
#  DISPLAYING USER DATA AND AVATARS LOADING
# ==========================================================================================
func LOADING_USERS_DATA():
	# get the local user
	var user = Steam.get_user()
	# display data about our user
	display_user(user)
	
	# load friends list
	var flist = Steam.get_friends(Steam.NOT_OFFLINE) # get online friends (default, same as without an argument)
	for friend in flist:
		display_user(friend)
	# we can also

func display_user( user ):
	if user == null or !(user extends SteamUser):
		print("Not user! (",user)
		return
	# prepare new data holder
	var panel = get_node("User").duplicate()
	panel.show()
	panel.get_node("Sprite").set_texture(null) # no avatar at start
	get_node("Container").add_child(panel)
	# declare some vars for later use
	var t = panel.get_node("Text")
	var utypes = ["INVALID","YOU","USER"] # User type constants
	var ustatus = ["OFFLINE","ONLINE","BUSY","AWAY","SNOOZE","LF_TRADE","LF_PLAY"] # user status constants
	# load data to vars (used them to make functions more visible)
	var type = user.get_user_type()
	var name = user.get_name()
	var status = user.get_status()
	var steamlevel = user.get_steamlevel() # steamlevel might not work for not YOU for some unknown reason
	var steam_id = user.get_steamID()
	var acc_id = user.get_accountID()
	var is_friend = user.is_friend()
	# display data
	t.add_text(str(utypes[type])) # YOU / USER
	if is_friend:
		t.add_text(" (FRIEND)") # USER (FRIEND)
	t.add_text("\n")
	t.add_text(str("  Name: ",name,"\n"))
	t.add_text(str("  Status: ",ustatus[status],"\n"))
	t.add_text(str("  Steam level: ",steamlevel,"\n"))
	t.add_text(str("  SteamID: ",steam_id,"\n"))
	t.add_text(str("  AccountID: ",acc_id))
	# load avatar
	var sprite = panel.get_node("Sprite")
	user.connect("avatar_loaded",self,"_on_avatar_loaded",[sprite])
	user.load_avatar(user.AVATAR_MEDIUM) # default arg, could use .load_avatar() as well
	# we're done. Avatar will show up as soon as it loads from SteamAPI

# "avatar_loaded" signal returns "size" and "avatar" arguments.
# "sprite" is our custom arg, added inside .connect()
func _on_avatar_loaded( size, avatar, sprite ):
	# make texture from our image
	var tex = ImageTexture.new()
	tex.create_from_image(avatar)
	# apply it to sprite
	sprite.set_texture(tex)








# ==========================================================================================
#  GROUPS
# ==========================================================================================
func GROUPS():
	# get array of groups you belong to
	var glist = Steam.get_groups()
	# it might surprising count of them
	# yet even by clicking "Watch" on some game you're being added to it's community group
	print("All groups count: ",glist.size())
	if glist.size() == 0: # you've got no groups
		return 
	# lets sort them
	var community_groups = []
	var groups = []
	for group in glist:
		if !group.has_tag(): # probably just a community group
			community_groups.push_back(group.get_name())
		else:
			groups.push_back(group.get_name())
	# print real groups
	print("Normal groups: ",groups)








# ==========================================================================================
#  OVERLAY
# ==========================================================================================
func OVERLAY():
	# first off lets check if we have access to our overlay
	# it may take second or few for overlay to load after process starts
	if !Steam.overlay_is_enabled():
		print("Waiting for overlay to load...")
		while(!Steam.overlay_is_enabled()):
			yield(get_tree(),"idle_frame")
	print("Overlay loaded.")
	# Set notifications position on overlay
	Steam.overlay_set_notification_pos( Steam.TOP_RIGHT )
	
	# I recommend to leave only 1 uncommented
	# Check API reference for more info
#	Steam.overlay_open("community") # leave blank to just activate overlay
#	Steam.overlay_open_user("steamid",Steam.get_user())
		# works same as
	Steam.overlay_open_url(Steam.get_user().get_profile_url())
#	Steam.overlay_open_store()








# ==========================================================================================
#  GAME_INFO
# ==========================================================================================
func GAME_INFO():
	# Update information about server you're on. 
	# It will be displayed after someone clicks "Join game" or "View game information"
	# arguments = (SteamID ServerID, server_ip, port)
	Steam.set_server_info(Steam.get_user(),"localhost",11234) # using user's SteamID as fake
	
	# Set information about the game. This is generally used by the users playing the same game
	# check api reference
	Steam.set_game_info("status","Testing Steam API for Godot Engine")
	var launch_cmd = "-connect \"127.0.0.1\":11234" # or something like that
	Steam.set_game_info("connect",launch_cmd)
	Steam.set_game_info("custom_key","custom_value")
	# this data can be later retrieved by others users using
	# SteamUser.get_game_info( key )
