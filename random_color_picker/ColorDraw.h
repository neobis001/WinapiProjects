#pragma once
#include "custom_button.h"
#include "ColorDC.h"

class ColorDraw;
typedef ColorDraw* PColorDraw;

class ColorDraw : public CSButton {
private:
	PColorDC connected_cdc;

public:
	ColorDraw();

	ColorDraw(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text);

	void connect_cdc(PColorDC connected_cdc);

	virtual RETURNMSG ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button);

};