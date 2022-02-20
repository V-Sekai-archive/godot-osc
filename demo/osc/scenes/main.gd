extends Node3D

func _ready():
	make_test_packet()
#	make_packet()
	
func make_test_packet():
	var osc : OscBuffer = OscBuffer.new()
	var osc_packet : Array[OscBundle] = osc._make_test_packet()
	print("osc packet %s" % osc_packet)
	var osc_buffer : PackedByteArray = osc.make_packet(osc_packet)
	print("osc buffer %s" % osc_buffer)
	osc.handle_packet(osc_buffer)

#func make_packet():
#	var osc : OscBuffer = OscBuffer.new()
#	var osc_packet : Array[OscBundle]	
#	var bundle : OscBundle = OscBundle.new()
#	var message_2 = OscMessage.new()
#	message_2.path = "/n_set";
#	var int_0 : OscArgumentInt32= OscArgumentInt32.new()
#	int_0.value = 1;
#	message_2.properties.push_back(int_0)
#	var osc_string : OscArgumentString  = OscArgumentString.new()
#	osc_string.instantiate();
#	osc_string.value = "wobble";
#	message_2.properties.push_back(osc_string);
#	var int_1 : OscArgumentInt32 = OscArgumentInt32.new()
#	int_1.value = 1;
#	message_2.properties.push_back(int_1);
#	var messages : Array[OscMessage];
#	messages.push_back(message_2);
#	bundle.time_code.instantiate();
#	bundle.time_code.value = 1234;
#	bundle.arguments = messages;
#	osc_packet.push_back(bundle)
#	print("osc packet %s" % osc_packet)
#	var osc_buffer : PackedByteArray = osc.make_packet(osc_packet)
#	print("osc buffer %s" % osc_buffer)
#	osc.handle_packet(osc_buffer)
