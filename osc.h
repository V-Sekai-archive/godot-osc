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

	uint64_t value = 0;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_value", "value"), &OscTimeCode::set_value);
		ClassDB::bind_method(D_METHOD("get_value"), &OscTimeCode::get_value);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
	}

public:
	void set_value(uint64_t p_value) {
		value = p_value;
	}
	uint64_t get_value() const {
		return value;
	}
};
class OscArgument : public OscType {
	GDCLASS(OscArgument, OscType);
};
class OscArgumentInt32 : public OscArgument {
	GDCLASS(OscArgumentInt32, OscArgument);

	int32_t value = 0;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_value", "value"), &OscArgumentInt32::set_value);
		ClassDB::bind_method(D_METHOD("get_value"), &OscArgumentInt32::get_value);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
	}

public:
	void set_value(int32_t p_value) {
		value = p_value;
	}
	int32_t get_value() const {
		return value;
	}
};
class OscArgumentFloat : public OscArgument {
	GDCLASS(OscArgumentFloat, OscArgument);

	float_t value = 0.0f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_value", "value"), &OscArgumentFloat::set_value);
		ClassDB::bind_method(D_METHOD("get_value"), &OscArgumentFloat::get_value);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "value"), "set_value", "get_value");
	}

public:
	void set_value(float_t p_value) {
		value = p_value;
	}
	float_t get_value() const {
		return value;
	}
};
class OscArgumentString : public OscArgument {
	GDCLASS(OscArgumentString, OscArgument);

	String value;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_value", "value"), &OscArgumentString::set_value);
		ClassDB::bind_method(D_METHOD("get_value"), &OscArgumentString::get_value);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "set_value", "get_value");
	}

public:
	void set_value(String p_value) {
		value = p_value;
	}
	String get_value() const {
		return value;
	}
};
class OscArgumentArray : public OscArgument {
	GDCLASS(OscArgumentArray, OscArgument);

	TypedArray<OscArgument> value;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_value", "value"), &OscArgumentArray::set_value);
		ClassDB::bind_method(D_METHOD("get_value"), &OscArgumentArray::get_value);
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "value", PROPERTY_HINT_ARRAY_TYPE,
						 vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "OscArgument"),
						 PROPERTY_USAGE_DEFAULT),
			"set_value", "get_value");
	}
public:
	void set_value(TypedArray<OscArgument> p_value) {
		value = p_value;
	}
	TypedArray<OscArgument> get_value() const {
		return value;
	}
};
class OscMessage : public OscType {
	GDCLASS(OscMessage, OscType);

	String path;
	TypedArray<OscArgument> properties;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_path", "path"), &OscMessage::set_path);
		ClassDB::bind_method(D_METHOD("get_path"), &OscMessage::get_path);
		ClassDB::bind_method(D_METHOD("set_properties", "properties"), &OscMessage::set_properties);
		ClassDB::bind_method(D_METHOD("get_properties"), &OscMessage::get_properties);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "path"), "set_path", "get_path");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "properties", PROPERTY_HINT_ARRAY_TYPE,
						 vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "OscArgument"),
						 PROPERTY_USAGE_DEFAULT),
			"set_properties", "get_properties");
	}

public:
	void set_path(String p_path) {
		path = p_path;
	}
	String get_path() const {
		return path;
	}
	void set_properties(TypedArray<OscArgument> p_properties) {
		properties = p_properties;
	}
	TypedArray<OscArgument> get_properties() const {
		return properties;
	}
};
class OscBundle : public OscType {
	GDCLASS(OscBundle, OscType);

	Ref<OscTimeCode> time_code;
	TypedArray<OscMessage> arguments;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_time_code", "time_code"), &OscBundle::set_time_code);
		ClassDB::bind_method(D_METHOD("get_time_code"), &OscBundle::get_time_code);
		ClassDB::bind_method(D_METHOD("set_osc_messages", "osc_messages"), &OscBundle::set_osc_messages);
		ClassDB::bind_method(D_METHOD("get_osc_messages"), &OscBundle::get_osc_messages);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "time_code", PROPERTY_HINT_RESOURCE_TYPE, "time_code", PROPERTY_USAGE_DEFAULT),
		"set_time_code", "get_time_code");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "osc_messages", PROPERTY_HINT_ARRAY_TYPE,
						 vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "OscMessage"),
						 PROPERTY_USAGE_DEFAULT),
			"set_osc_messages", "get_osc_messages");
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