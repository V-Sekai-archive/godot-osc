extends Node3D

func _ready():
	var osc : OscBuffer = OscBuffer.new()
	osc.handle_packet(osc.make_packet())
