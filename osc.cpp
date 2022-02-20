#include "osc.h"

void Osc::_bind_methods() {
	ClassDB::bind_method(D_METHOD("handle_packet", "bytes"), &Osc::handle_packet);
	ClassDB::bind_method(D_METHOD("make_packet"), &Osc::make_packet);
}

void Osc::handle_packet(Vector<uint8_t> p_bytes) {
	OSCPP::Server::Packet packet = OSCPP::Server::Packet(p_bytes.ptrw(), p_bytes.size());
	_handle_packet(packet);
}

void Osc::_handle_packet(OSCPP::Server::Packet p_packet) {
	if (p_packet.isBundle()) {
		OSCPP::Server::Bundle bundle(p_packet);
		print_line(vformat("#bundle %s", bundle.time()));
		OSCPP::Server::PacketStream packets(bundle.packets());
		while (!packets.atEnd()) {
			_handle_packet(packets.next());
		}
	} else {
		OSCPP::Server::Message msg(p_packet);
		OSCPP::Server::ArgStream args(msg.args());
		if (msg == "/s_new") {
			const char *name = args.string();
			const int32_t id = args.int32();
			print_line(vformat("/s_new %s %s ", name, id));
			OSCPP::Server::ArgStream params(args.array());
			while (!params.atEnd()) {
				const char *param = params.string();
				const float value = params.float32();
				print_line(vformat("%s %s", param, value));
			}
		} else if (msg == "/n_set") {
			const int32_t id = args.int32();
			const char *key = args.string();
			const float value = args.float32();
			print_line(vformat("/n_set %s %s %s", id, key, value));
		} else {
			print_line(vformat("Unknown message %s", msg.address()));
		}
	}
}

Vector<uint8_t> Osc::make_packet() {
	buffer.fill(0);
	OSCPP::Client::Packet packet(buffer.ptrw(), buffer.size());
	// Open a bundle with a timetag
	packet.openBundle(1234ULL)
			// Add a message with two arguments and an array with 6 elements;
			// for efficiency this needs to be known in advance.
			.openMessage("/s_new", 2 + OSCPP::Tags::array(6))
			// Write the arguments
			.string("sinesweep")
			.int32(2)
			.openArray()
			.string("start-freq")
			.float32(330.0f)
			.string("end-freq")
			.float32(990.0f)
			.string("amp")
			.float32(0.4f)
			.closeArray()
			// Every `open` needs a corresponding `close`
			.closeMessage()
			// Add another message with one argument
			.openMessage("/n_free", 1)
			.int32(1)
			.closeMessage()
			// And nother one
			.openMessage("/n_set", 3)
			.int32(1)
			.string("wobble")
			// Numeric arguments are converted automatically
			// (see below)
			.int32(31)
			.closeMessage()
			.closeBundle();
	return buffer.slice(0, packet.size());
}

Osc::Osc() {
	buffer.resize(OSC_MAX_PACKET_SIZE);
}