#pragma once
#include "custom_control.h"

class CSEdit;
typedef CSEdit* PCSEdit;

class CSEdit : public CSControl {
private:
	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent);

public:
	CSEdit();

	CSEdit(HINSTANCE hInstance, PCSWindow parent);
};