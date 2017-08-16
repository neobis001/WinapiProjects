#pragma once
#include "custom_control.h"

class CSUpdown;
typedef CSUpdown* PCSUpdown;

class CSUpdown : public CSControl {
private:
	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent);

public:
	CSUpdown();

	CSUpdown(HINSTANCE hInstance, PCSWindow parent);

	void set_range(int min_value, int max_value);

	void set_pos(int value);

	void set_buddy(PCSWindow buddy);

	INT get_pos();

};
