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
	Vector<Ref<OscArgument>> value;
};
class OscMessage : public OscType {
	GDCLASS(OscMessage, OscType);

public:
	String path;
	Vector<Ref<OscArgument>> properties;
};
class OscBundle : public OscType {
	GDCLASS(OscBundle, OscType);

public:
	Ref<OscTimeCode> time_code;
	Vector<Ref<OscMessage>> arguments;
};

class OscBuffer : public Resource {
	GDCLASS(OscBuffer, Resource);
	void _handle_packet(OSCPP::Server::Packet p_packet);
	const size_t OSC_MAX_PACKET_SIZE = 8192;
	Vector<uint8_t> buffer;

protected:
	static void _bind_methods();

public:
	Vector<uint8_t> _make_packet_binary(TypedArray<OscBundle> p_bundles);
	TypedArray<OscBundle> _make_packet();
	Vector<uint8_t> make_packet();
	void handle_packet(Vector<uint8_t> p_bytes);
	OscBuffer();
};
#endif // OSC_H