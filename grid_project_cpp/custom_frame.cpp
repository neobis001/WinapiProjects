#include "custom_frame.h"

bool CSFrame::class_registered = false;

void CSFrame::init_with_defaults(HINSTANCE hInstance, PCSWindow parent) {
	CSWNDEXOPS wops;
	wops.dwExStyle = 0;
	wops.lpClassName = frame_class;
	wops.lpWindowName = TEXT("CSFrame instance");
	wops.dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN; //note create_linked_hwnd adds WS_CHILD too, based
										  //on parent
	wops.x = 0;
	wops.y = 0;
	wops.nWidth = 300;
	wops.nHeight = 200;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	if (!class_registered) {
		CSWNDCLASSEX cswndclass;
		cswndclass.style = 0;
		cswndclass.cbClsExtra = 0;
		cswndclass.cbWndExtra = 0;
		cswndclass.hInstance = hInstance;
		cswndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		cswndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		cswndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		cswndclass.lpszMenuName = NULL;
		cswndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		init(cswndclass, wops, parent);
		class_registered = true;
	}
	else {
		init(wops, parent);
	}
}

CSFrame::CSFrame() {}

CSFrame::CSFrame(HINSTANCE hInstance, PCSWindow parent) {
	init_with_defaults(hInstance, parent);
}