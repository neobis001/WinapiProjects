#pragma once
#include "custom_control.h"

class CSButton;
typedef CSButton* PCSButton;


class CSButton : public CSControl {
protected:
	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text);

public:
	CSButton();

	CSButton(HINSTANCE hInstance, PCSWindow parent, DYNCSTR button_text);

};
