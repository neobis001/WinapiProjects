#pragma once
#include "custom_button.h"
#include "ColorSB.h"

class ColorRMR;
typedef ColorRMR* PColorRMR;

class ColorRMR : public CSButton {
private:
	PColorSB red_sb = nullptr;
	PColorSB green_sb = nullptr;
	PColorSB blue_sb = nullptr;

public:
	ColorRMR();

	ColorRMR(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text);

	void connect_sbs(PColorSB red_sb, PColorSB green_sb, PColorSB blue_sb);

	void randomize_sbs();

	virtual RETURNMSG ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button);

};