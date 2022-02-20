#ifndef OSC_H
#define OSC_H

#include "core/io/resource.h"
#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/typed_array.h"

#include <stdint.h>
#include <iostream>
#include <oscpp/print.hpp>
#include <oscpp/server.hpp>

class OscType : public RefCounted {
	GDCLASS(OscType, RefCounted);

public:
	OscType() {}
	~OscType() {}
};
class OscTimeCode : public OscType {
	GDCLASS(OscTimeCode, OscType);

public:
	uint64_t value = 0;
};
class OscArgumentType : public OscType {
	GDCLASS(OscArgumentType, OscType);
};
class OscArgument : public OscType {
	GDCLASS(OscArgument, OscType);
};
class OscArgumentInt32 : public OscArgument {
	GDCLASS(OscArgumentInt32, OscArgument);

public:
	int32_t value = 0;
};
class OscArgumentFloat : public OscArgument {
	GDCLASS(OscArgumentFloat, OscArgument);

public:
	float_t value = 0.0f;
};
class OscArgumentString : public OscArgument {
	GDCLASS(OscArgumentString, OscArgument);

public:
	String value;
};
class OscArgumentArray : public OscArgument {
	GDCLASS(OscArgumentArray, OscArgument);

public:
	TypedArray<OscArgument> value;
};
class OscMessage : public OscType {
	GDCLASS(OscMessage, OscType);

public:
	String path;
	TypedArray<OscArgument> properties;
};
class OscBundle : public OscType {
	GDCLASS(OscBundle, OscType);

	Ref<OscTimeCode> time_code;
	TypedArray<OscMessage> arguments;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_time_code", "time_code"), &OscBundle::set_time_code);
		ClassDB::bind_method(D_METHOD("get_time_code"), &OscBundle::get_time_code);
		ClassDB::bind_method(D_METHOD("set_osc_messages", "time_code"), &OscBundle::set_osc_messages);
		ClassDB::bind_method(D_METHOD("get_osc_messages"), &OscBundle::get_osc_messages);
	}

public:
	void set_time_code(Ref<OscTimeCode> p_time_code) {
		time_code = p_time_code;
	}
	Ref<OscTimeCode> get_time_code() const {
		return time_code;
	}
	void set_osc_messages(TypedArray<OscMessage> p_arguments) {
		arguments = p_arguments;
	}
	TypedArray<OscMessage> get_osc_messages() const {
		return arguments;
	}
};

class OscBuffer : public Resource {
	GDCLASS(OscBuffer, Resource);
	void _handle_packet(OSCPP::Server::Packet p_packet);
	const size_t OSC_MAX_PACKET_SIZE = 8192;
	Vector<uint8_t> buffer;

protected:
	static void _bind_methods();

public:
	int32_t _handle_arguments(Ref<OscArgument> p_args, OSCPP::Client::Packet *r_packet = nullptr);
	Vector<uint8_t> make_packet(TypedArray<OscBundle> p_bundles);
	TypedArray<OscBundle> _make_test_packet();
	void handle_packet(Vector<uint8_t> p_bytes);
	OscBuffer();
};
#endif // OSC_H