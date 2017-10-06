
extends Node

var steam
onready var textInfo = get_node("textInfo")
onready var textName = get_node("textName")
onready var btList = get_node("btList")
onready var btGet = get_node("btGet")
onready var btSet = get_node("btSet")
onready var btClear = get_node("btClear")

func _ready():
	btList.connect("pressed", self, "_onBtListPressed")
	btGet.connect("pressed", self, "_onBtGetPressed")
	btSet.connect("pressed", self, "_onBtSetPressed")
	btClear.connect("pressed", self, "_onBtClearPressed")
	steam = GodotSteam.new()
	if(steam.init()):
		print("Init steam api success.")
	else:
		print("Init steam api failed.")
		get_tree().quit()

func _onBtListPressed():
	var text = "Achievements list:\n"
	for i in range(steam.achievementsGetNum()):
		var name = steam.achievementGetName(i)
		text += name + "\n"
	textInfo.set_text(text)

func _onBtGetPressed():
	var text = "Is \"" + textName.get_text() + "\" achieved?:\n"
	var data = steam.achievementGet(textName.get_text())
	if(data.isSuccessGet):
		text += str(data.isAchieved) + "\n"
	else:
		text += "Failed\n"
	textInfo.set_text(text)

func _onBtSetPressed():
	var text = "Set \"" + textName.get_text() + "\":\n"
	var result = steam.achievementSet(textName.get_text())
	if(result):
		text += "Success\n"
	else:
		text += "Failed\n"
	textInfo.set_text(text)

func _onBtClearPressed():
	var text = "Clear \"" + textName.get_text() + "\":\n"
	var result = steam.achievementClear(textName.get_text())
	if(result):
		text += "Success\n"
	else:
		text += "Failed\n"
	textInfo.set_text(text)


