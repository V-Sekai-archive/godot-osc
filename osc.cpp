#include "osc.h"

#include <stdint.h>

void OscBuffer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("handle_packet", "bytes"), &OscBuffer::handle_packet);
	ClassDB::bind_method(D_METHOD("make_packet"), &OscBuffer::make_packet);
}

void OscBuffer::handle_packet(Vector<uint8_t> p_bytes) {
	OSCPP::Server::Packet packet = OSCPP::Server::Packet(p_bytes.ptrw(), p_bytes.size());
	_handle_packet(packet);
}

void OscBuffer::_handle_packet(OSCPP::Server::Packet p_packet) {
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

Vector<uint8_t> OscBuffer::make_packet() {
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

OscBuffer::OscBuffer() {
	buffer.resize(OSC_MAX_PACKET_SIZE);
}

Vector<Ref<OscBundle>> OscBuffer::_make_packet() {
	// Ref<OscMessage> message_0;
	// message_0.instantiate();
	// message_0->path = "/s_new";
	// OscArgumentString sine_sweep;
	// sine_sweep->value = "sinesweep";
	// OscArgumentInt32 sine_sweep_count;
	// sine_sweep_count.value = 2;
	// message_0->properties.push_back(sine_sweep);
	// message_0->properties.push_back(sine_sweep_count);
	// OscArgumentString start_freq;
	// start_freq.value = "start-freq";
	// OscArgumentInt32 start_freq_float;
	// start_freq_float.value = 330.0f;
	// Ref<OscArgumentArray> array;
	// array.push_back(start_freq);
	// array.push_back(start_freq_float);
	// message_0.properties.push_back(array);
	// OscMessage message_1;
	// message_1.path = "/n_free";
	// message_1.properties.push_back(OscArgumentInt32{ 1 });
	// OscArgumentInt32 end_freq;
	// end_freq.value = "end-freq";
	// OscArgumentInt32 end_freq_float;
	// end_freq_float.value = 990.0f;
	// array.push_back(end_freq);
	// array.push_back(end_freq_float);
	// OscArgumentInt32 amp;
	// amp.value = "amp";
	// OscArgumentInt32 amp_float;
	// amp_float.value = 0.4f;
	// array.push_back(amp);
	// array.push_back(amp_float);
	Ref<OscMessage> message_2;
	message_2.instantiate();
	message_2->path = "/n_set";
	Ref<OscArgumentInt32> int_0;
	int_0.instantiate();
	int_0->value = 1;
	message_2->properties.push_back(int_0);
	Ref<OscArgumentString> osc_string;
	osc_string.instantiate();
	osc_string->value = "wobble";
	message_2->properties.push_back(osc_string);
	Ref<OscArgumentInt32> int_1;
	int_1.instantiate();
	int_1->value = 1;
	message_2->properties.push_back(int_1);
	Vector<Ref<OscMessage>> messages;
	// messages.push_back(message_0);
	// messages.push_back(message_1);
	messages.push_back(message_2);
	Ref<OscBundle> bundle;
	bundle.instantiate();
	bundle->time_code.instantiate();
	bundle->time_code->value = 1234ULL;
	bundle->arguments = messages;
	Vector<Ref<OscBundle>> bundles;
	bundles.push_back(bundle);
	return bundles;
}

Vector<uint8_t> OscBuffer::_make_packet_binary(Vector<Ref<OscBundle>> p_bundles) {
	buffer.fill(0);
	OSCPP::Client::Packet packet(buffer.ptrw(), buffer.size());
	for (Ref<OscBundle> bundle : p_bundles) {
		packet = packet.openBundle(bundle->time_code->value);
		if (bundle.is_valid()) {
			for (Ref<OscMessage> message : bundle->arguments) {
				int32_t count = 0;
				if (message.is_valid()) {
					for (Ref<OscArgument> arg : message->properties) {
					}
				}
				packet = packet.openMessage(message->path.ascii().get_data(), count);
				packet = packet.closeMessage();
			}
		}
		packet = packet.closeBundle();
	}
	return buffer.slice(0, packet.size());
	// // Add a message with two arguments and an array with 6 elements;
	// // for efficiency this needs to be known in advance.
	// .openMessage("/s_new", 2 + OSCPP::Tags::array(6))
	// 		// Write the arguments
	// 		.string("sinesweep")
	// 		.int32(2)
	// 		.openArray()
	// 		.string("start-freq")
	// 		.float32(330.0f)
	// 		.string("end-freq")
	// 		.float32(990.0f)
	// 		.string("amp")
	// 		.float32(0.4f) return buffer.slice(0, packet.size());
}