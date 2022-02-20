
#ifndef OSC_H
#define OSC_H

#include "core/object/ref_counted.h"
#include <stdint.h>

#include <iostream>
#include <oscpp/print.hpp>
#include <oscpp/server.hpp>

class Osc : public RefCounted {
	GDCLASS(Osc, RefCounted);
	void _handle_packet(OSCPP::Server::Packet p_packet);

	const size_t OSC_MAX_PACKET_SIZE = 8192;

	Vector<uint8_t> buffer;

protected:
	static void _bind_methods();

public:

/* Packet layout in `nomnoml` markup language
 * [Byte buffer] -> [OSCPP::Client::Packet]
 * [OSCPP::Client::Packet] -> [Bundle]
 * [Bundle] -> [Message]
 * [Message] -> [Arguments | string | int32 | float32 | array] */
	Vector<uint8_t> make_packet();
	void handle_packet(Vector<uint8_t> p_bytes);
	Osc();
};

#endif // OSC_H