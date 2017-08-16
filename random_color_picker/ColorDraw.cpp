#include "ColorDraw.h"

ColorDraw::ColorDraw() {

}

ColorDraw::ColorDraw(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text) {
	init_with_defaults(hInstance, parent, button_text);
}

void ColorDraw::connect_cdc(PColorDC connected_cdc) {
	this->connected_cdc = connected_cdc;
}

RETURNMSG ColorDraw::ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button) {
	connected_cdc->update_drawing_colors();
	return create_default_option(0);
}