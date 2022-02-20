extends Node3D

func _ready():
	var osc : Osc = Osc.new()
	osc.handle_packet(osc.make_packet())
