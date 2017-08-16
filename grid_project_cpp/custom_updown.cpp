#include "custom_updown.h"

void CSUpdown::init_with_defaults(HINSTANCE hInstance, PCSWindow parent) {
	initialize_common_controls(ICC_UPDOWN_CLASS);

	CSWNDEXOPS wops;
	wops.dwExStyle = 0;
	wops.lpClassName = UPDOWN_CLASS;
	wops.lpWindowName = TEXT("");
	wops.dwStyle = UDS_SETBUDDYINT | UDS_ARROWKEYS | 
		UDS_ALIGNRIGHT | WS_VISIBLE | WS_CHILD ; //add_wops_defaults considers WS_CHILD and WS_CLIPCHILDREN
	wops.x = 0;
	wops.y = 0;
	wops.nWidth = 100;
	wops.nHeight = 100;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	init(wops, parent);
}

CSUpdown::CSUpdown() {

}

CSUpdown::CSUpdown(HINSTANCE hInstance, PCSWindow parent) {
	init_with_defaults(hInstance, parent);
}

void CSUpdown::set_range(int min_value, int max_value) {
	SendMessage(hwnd, UDM_SETRANGE, 0, MAKELPARAM(max_value, min_value));
}

void CSUpdown::set_pos(int value) {
	SendMessage(hwnd, UDM_SETPOS, 0, (LPARAM)value);
}

void CSUpdown::set_buddy(PCSWindow buddy) {
	SendMessage(hwnd, UDM_SETBUDDY, (WPARAM)buddy->get_hwnd(), 0);
}

INT CSUpdown::get_pos() {
	LRESULT res = SendMessage(hwnd, UDM_GETPOS, 0, 0);
	UINT error_code = HIWORD(res);
	INT value = LOWORD(res);

	if (error_code != 0) {
		msg_string("Warning (in CSUpdown::get_updown_pos): UDM_GETPOS message result showed failure"\
			", will force quite while returning -1");
		return -1;
	}
	else {
		return value;
	}
}