#pragma once
#include "custom_spinbox.h"
#include <cstdlib>

class ColorSB;
typedef ColorSB* PColorSB;

class ColorSB : public CSSpinbox {
private:
	static const int min_component;
	static const int max_component;

protected:
	virtual void add_wops_defaults();

public:
	ColorSB();

	ColorSB(HINSTANCE hInstance, PCSWindow parent, int start);

	void randomize_component();

};