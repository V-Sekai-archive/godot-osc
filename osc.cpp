
#include "osc.h"

void Osc::add(int p_value) {
	count += p_value;
}

void Osc::reset() {
	count = 0;
}

int Osc::get_total() const {
	return count;
}

void Osc::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add", "value"), &Osc::add);
	ClassDB::bind_method(D_METHOD("reset"), &Osc::reset);
	ClassDB::bind_method(D_METHOD("get_total"), &Osc::get_total);
}

Osc::Osc() {
	count = 0;
}