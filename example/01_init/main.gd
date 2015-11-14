
extends Node

var steam

func _ready():
	steam = GodotSteam.new()
	if(steam.init()):
		print("Init steam api success.")
	else:
		print("Init steam api failed.")
		get_tree().quit()



