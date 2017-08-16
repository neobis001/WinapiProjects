#include "ColorSB.h"


void ColorSB::add_wops_defaults() {
	stored_wops.dwStyle |= ES_READONLY;
	CSSpinbox::add_wops_defaults();
}

const int ColorSB::min_component = 0;
const int ColorSB::max_component = 255;

ColorSB::ColorSB() {

}

ColorSB::ColorSB(HINSTANCE hInstance, PCSWindow parent, int start) {
	init_with_defaults(hInstance, parent);

	PCSEdit buddy = new CSEdit(hInstance, this);
	PCSUpdown ud = new CSUpdown(hInstance, this);
	setup_sb_windows(buddy, ud, min_component, max_component, start);
}

void ColorSB::randomize_component() {
	int random_num = rand() % (max_component + 1);
	updown->set_pos(random_num);
}