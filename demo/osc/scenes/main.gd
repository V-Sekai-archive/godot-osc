extends Node3D

var udp_peer: PacketPeerUDP

func udp_listen(udp_port: int):
	if udp_peer != null:
		udp_peer.close()
	udp_peer = PacketPeerUDP.new()
	udp_peer.listen(udp_port)

func udp_connect_to_host(host: String, port: int):
	if udp_peer == null:
		udp_peer = PacketPeerUDP.new()
	udp_peer.connect_to_host(host, port)

func udp_set_dest_address(host: String, port: int):
	if udp_peer == null:
		udp_peer = PacketPeerUDP.new()
	udp_peer.set_dest_address(host, port)

func _ready():
#	make_test_packet()
	make_packet()
	
func make_test_packet():
	var osc : OscBuffer = OscBuffer.new()
	var osc_packet : Array[OscBundle] = osc._make_test_packet()
	print("osc packet %s" % osc_packet)
	var osc_buffer : PackedByteArray = osc.make_packet(osc_packet)
	print("osc buffer %s" % osc_buffer)
	osc.handle_packet(osc_buffer)

func make_packet():
	var osc : OscBuffer = OscBuffer.new()
	var bundle : OscBundle = OscBundle.new()
	var message_2 = OscMessage.new()
	message_2.path = "/n_set";
	var int_0 : OscArgumentInt32= OscArgumentInt32.new()
	int_0.value = 1;
	message_2.properties.push_back(int_0)
	var osc_string : OscArgumentString  = OscArgumentString.new()
	osc_string.value = "wobble";
	message_2.properties.push_back(osc_string);
	var int_1 : OscArgumentInt32 = OscArgumentInt32.new()
	int_1.value = 2;
	message_2.properties.push_back(int_1);
	var osc_array : OscArgumentArray = OscArgumentArray.new()
	osc_array.value.push_back(osc_string)
	osc_array.value.push_back(int_1)
	message_2.properties.push_back(osc_array);
	var time_code : OscTimeCode = OscTimeCode.new()
	time_code.value = 1234;
	bundle.time_code = time_code;
	bundle.osc_messages.push_back(message_2)
	var osc_packet : Array[OscBundle]
	osc_packet.push_back(bundle)
	print("osc packet %s" % osc_packet)
	var osc_buffer : PackedByteArray = osc.make_packet(osc_packet)
	print("osc buffer %s" % osc_buffer)
	osc.handle_packet(osc_buffer)
