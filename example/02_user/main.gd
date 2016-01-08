extends Node



func _ready():
	var steam = GodotSteam.new()
	# if we use .get_user() before init, it will be just empty GSteamUser
	var user = steam.get_user()
	print("Is 1. user empty? ",user.is_empty(),", ID:",user.get_id())
	# Initialize Steam
	steam.init()
	user = steam.get_user()
	print("Is 2. user empty? ",user.is_empty(),", ID:",user.get_id())
	print("Logged on? ",user.is_logged())


