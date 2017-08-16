#include "custom_edit.h"

void CSEdit::init_with_defaults(HINSTANCE hInstance, PCSWindow parent) {
	CSWNDEXOPS wops;
	wops.dwExStyle = WS_EX_CLIENTEDGE;
	wops.lpClassName = TEXT("EDIT");
	wops.lpWindowName = TEXT("");
	wops.dwStyle = WS_VISIBLE; //add_wops_defaults considers WS_CHILD and WS_CLIPCHILDREN
	wops.x = 0;
	wops.y = 0;
	wops.nWidth = 200;
	wops.nHeight = 100;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	init(wops, parent);
}

CSEdit::CSEdit() {

}

CSEdit::CSEdit(HINSTANCE hInstance, PCSWindow parent) {
	init_with_defaults(hInstance, parent);
}