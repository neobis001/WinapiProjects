#include "custom_control.h"


void CSControl::init_with_identifier(CSWNDEXOPS wops, PCSWindow parent) {
	ctl_identifier = available_ctl_id;
	wops.hMenu = (HMENU)ctl_identifier;
	available_ctl_id++;

	CSWindow::init(wops, parent);
}

void CSControl::init(CSWNDEXOPS wops, PCSWindow parent) {
	init_with_identifier(wops, parent);
}

unsigned short int CSControl::available_ctl_id = STARTING_CTL_IDENTIFIER;

CSControl::CSControl() {

}

int CSControl::get_id() {
	return ctl_identifier;
}