#include "osc.h"

#include <stdint.h>

void OscBuffer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("make_packet", "bundles"), &OscBuffer::make_packet);
	ClassDB::bind_method(D_METHOD("_make_test_packet"), &OscBuffer::_make_test_packet);
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

TypedArray<OscBundle> OscBuffer::_make_test_packet() {
	Ref<OscMessage> message_0;
	{
		message_0.instantiate();
		message_0->set_path("/s_new");
		Ref<OscArgumentString> sine_sweep;
		sine_sweep.instantiate();
		sine_sweep->set_value("sinesweep");
		Ref<OscArgumentInt32> sine_sweep_count;
		sine_sweep_count.instantiate();
		sine_sweep_count->set_value(2);
		TypedArray<OscArgument> value_0;
		value_0.push_back(sine_sweep);
		value_0.push_back(sine_sweep_count);
		Ref<OscArgumentString> start_freq;
		start_freq.instantiate();
		start_freq->set_value("start-freq");
		Ref<OscArgumentFloat> start_freq_float;
		start_freq_float.instantiate();
		start_freq_float->set_value(330.0f);
		Ref<OscArgumentString> end_freq;
		end_freq.instantiate();
		end_freq->set_value("end-freq");
		Ref<OscArgumentFloat> end_freq_float;
		end_freq_float.instantiate();
		end_freq_float->set_value(990.0f);
		Ref<OscArgumentString> amp;
		amp.instantiate();
		amp->set_value("amp");
		Ref<OscArgumentFloat> amp_float;
		amp_float.instantiate();
		amp_float->set_value(0.4f);
		TypedArray<OscArgument> value_array;
		value_array.push_back(start_freq);
		value_array.push_back(start_freq_float);
		value_array.push_back(end_freq);
		value_array.push_back(end_freq_float);
		value_array.push_back(amp);
		value_array.push_back(amp_float);
		Ref<OscArgumentArray> array;
		array.instantiate();
		array->set_value(value_array);
		value_0.push_back(array);
		message_0->set_properties(value_0);
	}
	Ref<OscMessage> message_1;
	{
		message_1.instantiate();
		message_1->set_path("/n_free");
		TypedArray<OscArgument> properties;
		Ref<OscArgumentInt32> int_32;
		int_32.instantiate();
		int_32->set_value(1);
		properties.push_back(int_32);
		message_1->set_properties(properties);
	}

	Ref<OscMessage> message_2;
	{
		message_2.instantiate();
		message_2->set_path("/n_set");
		Ref<OscArgumentInt32> int_0;
		int_0.instantiate();
		int_0->set_value(1);
		TypedArray<OscArgument> properties;
		properties.push_back(int_0);
		Ref<OscArgumentString> osc_string;
		osc_string.instantiate();
		osc_string->set_value("wobble");
		properties.push_back(osc_string);
		Ref<OscArgumentInt32> int_1;
		int_1.instantiate();
		int_1->set_value(1);
		properties.push_back(int_1);
		message_2->set_properties(properties);
	}
	TypedArray<OscMessage> messages;
	messages.push_back(message_0);
	messages.push_back(message_1);
	messages.push_back(message_2);
	Ref<OscBundle> bundle;
	{
		bundle.instantiate();
		Ref<OscTimeCode> time_code;
		time_code.instantiate();
		time_code->set_value(1234ULL);
		bundle->set_time_code(time_code);
		bundle->set_osc_messages(messages);
	}
	TypedArray<OscBundle> bundles;
	bundles.push_back(bundle);
	return bundles;
}

Vector<uint8_t> OscBuffer::make_packet(TypedArray<OscBundle> p_bundles) {
	buffer.fill(0);
	OSCPP::Client::Packet packet(buffer.ptrw(), buffer.size());
	for (int32_t bundle_i = 0; bundle_i < p_bundles.size(); bundle_i++) {
		Variant bundle = p_bundles[bundle_i];
		Ref<OscBundle> osc_bundle = cast_to<OscBundle>(bundle);
		if (osc_bundle.is_null()) {
			continue;
		}
		if (osc_bundle->get_time_code().is_null()) {
			continue;
		}
		packet = packet.openBundle(osc_bundle->get_time_code()->get_value());
		for (int32_t message_i = 0; message_i < osc_bundle->get_osc_messages().size(); message_i++) {
			Ref<OscMessage> message = osc_bundle->get_osc_messages()[message_i];
			int32_t count = 0;
			if (message.is_null()) {
				continue;
			}
			for (int32_t property_i = 0; property_i < message->get_properties().size(); property_i++) {
				Ref<OscArgument> arg = message->get_properties()[property_i];
				if (arg.is_null()) {
					continue;
				}
				count += _handle_arguments(arg, nullptr);
			}
			if (!count) {
				continue;
			}
			CharString cs = message->get_path().ascii();
			if (!cs.length()) {
				continue;
			}
			packet = packet.openMessage(cs.get_data(), count);
			for (int32_t property_i = 0; property_i < message->get_properties().size(); property_i++) {
				Ref<OscArgumentArray> array_arg = message->get_properties()[property_i];
				Ref<OscArgument> arg = message->get_properties()[property_i];
				if (arg.is_null()) {
					continue;
				}
				if (array_arg.is_null()) {
					_handle_arguments(arg, &packet);
				} else {
					packet = packet.openArray();
					for (int32_t value_i = 0; value_i < array_arg->get_value().size(); value_i++) {
						Ref<OscArgument> value_arg = array_arg->get_value()[value_i];
						_handle_arguments(value_arg, &packet);
					}
					packet = packet.closeArray();
				}
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
			*r_packet = r_packet->int32(osc_int->get_value());
		}
		return 1;
	} else if (osc_float_32.is_valid()) {
		if (r_packet) {
			*r_packet = r_packet->float32(osc_float_32->get_value());
		}
		return 1;
	} else if (osc_string.is_valid()) {
		if (r_packet) {
			CharString cs = osc_string->get_value().ascii();
			*r_packet = r_packet->string(cs.get_data());
		}
		return 1;
	} else if (osc_array.is_valid()) {
		int32_t count = 0;
		for (int32_t array_i = 0; array_i < osc_array->get_value().size(); array_i++) {
			Ref<OscArgument> arg = osc_array->get_value()[array_i];
			count += _handle_arguments(arg, r_packet);
		}
		return OSCPP::Tags::array(count);
	}
	return 0;
}