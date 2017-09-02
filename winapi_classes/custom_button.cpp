#include "custom_button.h"

void CSButton::init_with_defaults(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text) {
	CSWNDEXOPS wops;
	wops.dwExStyle = 0;
	wops.lpClassName = TEXT("BUTTON");
	wops.lpWindowName = button_text;
	wops.dwStyle = WS_VISIBLE | BS_MULTILINE; //add_wops_defaults considers WS_CHILD and WS_CLIPCHILDREN
	wops.x = 0;
	wops.y = 0;
	wops.nWidth = 200;
	wops.nHeight = 100;
	wops.hMenu = 0; //will be handled by CSControl init
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	init(wops, parent);
}

CSButton::CSButton() {

}

CSButton::CSButton(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text) {
	init_with_defaults(hInstance, parent, button_text);
}

LRESULT CALLBACK buttonSubclassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {

	switch (uMsg) {
	case BN_CLICKED: {
		msg_here("in BN_CLICKED");
	}
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}