extends Node3D

func _ready():
	var osc : OscBuffer = OscBuffer.new()
	var osc_packet : Array[OscBundle] = osc._make_test_packet()
	print("osc packet %s" % osc_packet)
	var osc_buffer : PackedByteArray = osc.make_packet(osc_packet)
	print("osc buffer %s" % osc_buffer)
	osc.handle_packet(osc_buffer)
