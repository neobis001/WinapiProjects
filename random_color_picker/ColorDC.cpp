#include "ColorDC.h"

void ColorDC::init_with_defaults(HINSTANCE hInstance, PCSWindow parent, int initial_width,
	int initial_height) {
	CSWNDEXOPS wops;
	wops.dwExStyle = 0;
	wops.lpClassName = colordc_class;
	wops.lpWindowName = TEXT("CSFrame instance");
	wops.dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN; //note create_linked_hwnd adds WS_CHILD too, based
															//on parent
	wops.x = 0;
	wops.y = 0;
	wops.nWidth = initial_width;
	wops.nHeight = initial_height;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	if (!class_registered) {
		CSWNDCLASSEX cswndclass;
		cswndclass.style = CS_HREDRAW | CS_VREDRAW;
		cswndclass.cbClsExtra = 0;
		cswndclass.cbWndExtra = 0;
		cswndclass.hInstance = hInstance;
		cswndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		cswndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		cswndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		cswndclass.lpszMenuName = NULL;
		cswndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		init(cswndclass, wops, parent);
		class_registered = true;
	}
	else {
		init(wops, parent);
	}
}

INT ColorDC::get_updown_pos(PColorSB csb) {
	return csb->get_spinbox_pos();
}

void ColorDC::update_background_brush() {
	INT r = get_updown_pos(bk_redb);
	INT g = get_updown_pos(bk_greenb);
	INT b = get_updown_pos(bk_blueb);

	DeleteObject(bk_brush); 
	bk_brush = CreateSolidBrush(RGB(r, g, b));
}

void ColorDC::update_circle_brush() {
	INT r = get_updown_pos(c_redb);
	INT g = get_updown_pos(c_greenb);
	INT b = get_updown_pos(c_blueb);

	DeleteObject(c_brush);
	c_brush = CreateSolidBrush(RGB(r, g, b));
}

void ColorDC::update_drawing_colors() {
	update_background_brush();
	update_circle_brush();
	visual_redraw();
}

void ColorDC::set_circle_padding(int padding) {
	if (padding < 0) {
		DYNCHAR buffer[200];
		wsprintf(buffer, "Error (in ColorDC::set_circle_padding): padding is negative (%i)", padding);
		msg_force_quit(buffer);
	}
	else {
		circle_padding = padding;
	}
}

bool ColorDC::class_registered = false;

ColorDC::ColorDC() {

}

ColorDC::ColorDC(HINSTANCE hInstance, PCSWindow parent, int initial_width, int initial_height) {
	init_with_defaults(hInstance, parent, initial_width, initial_height);
}

ColorDC::~ColorDC() {
	DeleteObject(bk_brush);
	DeleteObject(c_brush);
}

void ColorDC::connect_bk_sbs(PColorSB bk_redb, PColorSB bk_greenb, PColorSB bk_blueb) {
	this->bk_redb = bk_redb;
	this->bk_greenb = bk_greenb;
	this->bk_blueb = bk_blueb;
}

void ColorDC::connect_circle_sbs(PColorSB c_redb, PColorSB c_greenb, PColorSB c_blueb) {
	this->c_redb = c_redb;
	this->c_greenb = c_greenb;
	this->c_blueb = c_blueb;
}

RETURNMSG ColorDC::ON_WM_ERASEBKGND(HDC hdc) {
	return create_default_option(1); //return non-zero to tell app. not to erase bkgrnd on its own
}

RETURNMSG ColorDC::ON_WM_PAINT() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	LONG client_width = get_client_width();
	LONG client_height = get_client_height();


	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memB = CreateCompatibleBitmap(hdc, client_width, client_height);
	HBITMAP old_memB = (HBITMAP)SelectObject(memDC, memB);

	
	RECT rect;
	GetClientRect(hwnd, &rect);
	FillRect(memDC, &rect, bk_brush);


	//
	//
	//ellipse start
	LONG circle_diameter = get_min_value(client_width, client_height);
	if (circle_padding * 2 < circle_diameter) { //if overall padding doesn't swallow entire circle

		LONG circle_radius = circle_diameter / 2;
		int center_x = client_width / 2;
		int center_y = client_height / 2;

		RECT circle_rect;

		circle_rect.left = center_x - circle_radius;
		circle_rect.right = center_x + circle_radius;
		circle_rect.top = center_y - circle_radius;
		circle_rect.bottom = center_y + circle_radius;

		InflateRect(&circle_rect, -circle_padding, -circle_padding);
		HRGN circle = CreateEllipticRgnIndirect(&circle_rect);
		FillRgn(memDC, circle, c_brush);

		DeleteObject(circle);
	}
	//
	//
	//ellipse end

	BitBlt(hdc, 0, 0, client_width, client_height, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, old_memB);
	DeleteObject(memB);
	DeleteDC(memDC);

	EndPaint(hwnd, &ps);

	return create_default_option(0);
}