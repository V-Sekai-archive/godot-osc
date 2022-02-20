#include "osc.h"

#include <stdint.h>

void OscBuffer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_make_packet_binary", "bundles"), &OscBuffer::_make_packet_binary);
	ClassDB::bind_method(D_METHOD("_make_packet"), &OscBuffer::_make_packet);
	ClassDB::bind_method(D_METHOD("handle_packet", "bytes"), &OscBuffer::handle_packet);
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

OscBuffer::OscBuffer() {
	buffer.resize(OSC_MAX_PACKET_SIZE);
}

TypedArray<OscBundle> OscBuffer::_make_packet() {
	Ref<OscMessage> message_0;
	message_0.instantiate();
	message_0->path = "/s_new";
	Ref<OscArgumentString> sine_sweep;
	sine_sweep.instantiate();
	sine_sweep->value = "sinesweep";
	Ref<OscArgumentInt32> sine_sweep_count;
	sine_sweep_count.instantiate();
	sine_sweep_count->value = 2;
	message_0->properties.push_back(sine_sweep);
	message_0->properties.push_back(sine_sweep_count);
	Ref<OscArgumentString> start_freq;
	start_freq.instantiate();
	start_freq->value = "start-freq";
	Ref<OscArgumentFloat> start_freq_float;
	start_freq_float.instantiate();
	start_freq_float->value = 330.0f;
	Ref<OscArgumentArray> array;
	array.instantiate();
	array->value.push_back(start_freq);
	array->value.push_back(start_freq_float);
	message_0->properties.push_back(array);
	Ref<OscMessage> message_1;
	message_1.instantiate();
	message_1->path = "/n_free";
	Ref<OscArgumentInt32> int_32;
	int_32.instantiate();
	int_32->value = 1;
	message_1->properties.push_back(int_32);
	Ref<OscArgumentString> end_freq;
	end_freq.instantiate();
	end_freq->value = "end-freq";
	Ref<OscArgumentFloat> end_freq_float;
	end_freq_float.instantiate();
	end_freq_float->value = 990.0f;
	array->value.push_back(end_freq);
	array->value.push_back(end_freq_float);
	Ref<OscArgumentString> amp;
	amp.instantiate();
	amp->value = "amp";
	Ref<OscArgumentInt32> amp_float;
	amp_float.instantiate();
	amp_float->value = 0.4f;
	array->value.push_back(amp);
	array->value.push_back(amp_float);
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
	messages.push_back(message_0);
	messages.push_back(message_1);
	messages.push_back(message_2);
	Ref<OscBundle> bundle;
	bundle.instantiate();
	bundle->time_code.instantiate();
	bundle->time_code->value = 1234ULL;
	bundle->arguments = messages;
	TypedArray<OscBundle> bundles;
	bundles.push_back(bundle);
	return bundles;
}

Vector<uint8_t> OscBuffer::_make_packet_binary(TypedArray<OscBundle> p_bundles) {
	buffer.fill(0);
	OSCPP::Client::Packet packet(buffer.ptrw(), buffer.size());
	for (int32_t bundle_i = 0; bundle_i < p_bundles.size(); bundle_i++) {
		Variant bundle = p_bundles[bundle_i];
		Ref<OscBundle> osc_bundle = cast_to<OscBundle>(bundle);
		if (osc_bundle.is_null()) {
			continue;
		}
		packet = packet.openBundle(osc_bundle->time_code->value);
		for (Ref<OscMessage> message : osc_bundle->arguments) {
			int32_t count = 0;
			if (message.is_null()) {
				continue;
			}
			for (Ref<OscArgument> arg : message->properties) {
				count += _handle_arguments(arg, nullptr);
			}
			if (!count) {
				continue;
			}
			CharString cs = message->path.ascii();
			if (!cs.length()) {
				continue;
			}
			packet = packet.openMessage(cs.get_data(), count);
			for (Ref<OscArgument> arg : message->properties) {
				_handle_arguments(arg, &packet);
			}
			packet = packet.closeMessage();
		}
		packet = packet.closeBundle();
	}
	return buffer.slice(0, packet.size());
}

int32_t OscBuffer::_handle_arguments(Ref<OscArgument> p_args, OSCPP::Client::Packet *r_packet) {
	Ref<OscArgumentInt32> osc_int = p_args;
	Ref<OscArgumentFloat> osc_float_32 = p_args;
	Ref<OscArgumentString> osc_string = p_args;
	Ref<OscArgumentArray> osc_array = p_args;
	if (osc_int.is_valid()) {
		if (r_packet) {
			*r_packet = r_packet->int32(osc_int->value);
		}
		return 1;
	} else if (osc_float_32.is_valid()) {
		if (r_packet) {
			*r_packet = r_packet->int32(osc_float_32->value);
		}
		return 1;
	} else if (osc_string.is_valid()) {
		if (r_packet) {
			CharString cs = osc_string->value.ascii();
			*r_packet = r_packet->string(cs.get_data());
		}
		return 1;
	} else if (osc_array.is_valid()) {
		if (r_packet) {
			*r_packet = r_packet->openArray();
		}
		int32_t count = 0;
		for (Ref<OscArgument> arg : osc_array->value) {
			count += _handle_arguments(arg, r_packet);
		}
		if (r_packet) {
			*r_packet = r_packet->closeArray();
		}
		return OSCPP::Tags::array(count);
	}
	return 0;
}