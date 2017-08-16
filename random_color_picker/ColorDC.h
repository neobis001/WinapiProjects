#pragma once
#include "custom_window.h"
#include "ColorSB.h"

class ColorDC;
typedef ColorDC* PColorDC;

class ColorDC: public CSWindow {
private:
	static bool class_registered;
	DYNCSTR colordc_class = TEXT("COLORDC_CLASS");

	int circle_padding = 0;

	HBRUSH bk_brush = CreateSolidBrush(RGB(0,0,0));
	HBRUSH c_brush = CreateSolidBrush(RGB(255,255,255));

	PColorSB bk_redb = nullptr;
	PColorSB bk_greenb = nullptr;
	PColorSB bk_blueb = nullptr;

	PColorSB c_redb = nullptr;
	PColorSB c_greenb = nullptr;
	PColorSB c_blueb = nullptr;

	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent, int initial_width,
		int initial_height);

	INT get_updown_pos(PColorSB csb);

	void update_background_brush();

	void update_circle_brush();

public:
	ColorDC();

	~ColorDC();

	ColorDC(HINSTANCE hInstance, PCSWindow parent, int initial_width, int initial_height);

	void connect_bk_sbs(PColorSB bk_redb, PColorSB bk_greenb, PColorSB bk_blueb);

	void connect_circle_sbs(PColorSB c_redb, PColorSB c_greenb, PColorSB c_blueb);

	void update_drawing_colors();

	void set_circle_padding(int padding);

	virtual RETURNMSG ON_WM_ERASEBKGND(HDC hdc);

	virtual RETURNMSG ON_WM_PAINT();
};