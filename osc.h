
#ifndef OSC_H
#define OSC_H

#include "core/object/ref_counted.h"

class Osc : public RefCounted {
	GDCLASS(Osc, RefCounted);

	int count = 0;

protected:
	static void _bind_methods();

public:
	void add(int p_value);
	void reset();
	int get_total() const;

	Osc();
};

#endif // OSC_H