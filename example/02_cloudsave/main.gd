
extends Node

var steam
onready var textInfo = get_node("textInfo")
onready var textFileName = get_node("textFileName")
onready var btList = get_node("btList")
onready var btLoad = get_node("btLoad")
onready var btSave = get_node("btSave")
onready var btDelete = get_node("btDelete")

func _ready():
	btList.connect("pressed", self, "_onBtListPressed")
	btLoad.connect("pressed", self, "_onBtLoadPressed")
	btSave.connect("pressed", self, "_onBtSavePressed")
	btDelete.connect("pressed", self, "_onBtDeletePressed")
	steam = GodotSteam.new()
	if(steam.init()):
		print("Init steam api success.")
	else:
		print("Init steam api failed.")
		get_tree().quit()

func _onBtListPressed():
	var text = "Files list:\n"
	for i in range(steam.cloudGetCount()):
		var info = steam.cloudGetFileNameAndSize(i)
		text += info.fileName + " (" + str(info.fileSize) + " bytes)\n"
	textInfo.set_text(text)

func _onBtLoadPressed():
	var text = "Load file \"" + textFileName.get_text() + "\":\n"
	var data = steam.cloudFileRead(textFileName.get_text())
	if(data == null):
		text += "Failed\n"
	else:
		text += str(data) + "\n"
	textInfo.set_text(text)

func _onBtSavePressed():
	var text = "Save file \"" + textFileName.get_text() + "\":\n"
	var result = steam.cloudFileWrite(textFileName.get_text(), textInfo.get_text())
	if(result):
		text += "Success\n"
	else:
		text += "Failed\n"
	textInfo.set_text(text)

func _onBtDeletePressed():
	var text = "Delete file \"" + textFileName.get_text() + "\":\n"
	var result = steam.cloudFileDelete(textFileName.get_text())
	if(result):
		text += "Success\n"
	else:
		text += "Failed\n"
	textInfo.set_text(text)


