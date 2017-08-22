#include "ColorRMR.h"

ColorRMR::ColorRMR() {

}

ColorRMR::ColorRMR(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text) {
	init_with_defaults(hInstance, parent, button_text);
}

void ColorRMR::connect_sbs(PColorSB red_sb, PColorSB green_sb, PColorSB blue_sb) {
	this->red_sb = red_sb;
	this->green_sb = green_sb;
	this->blue_sb = blue_sb;
}

void ColorRMR::randomize_sbs() {
	if (red_sb == nullptr || green_sb == nullptr || blue_sb == nullptr) {
		msg_force_quit("Error (in ColorRMR::randomize_sbs): one of the sb variables is nullptr");
	}
	else {
		red_sb->randomize_component();
		green_sb->randomize_component();
		blue_sb->randomize_component();
	}
}

RETURNMSG ColorRMR::ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button) {
	randomize_sbs();
	return create_default_option(0); //WM_COMMAND processing should return 0
}