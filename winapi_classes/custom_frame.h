#pragma once
#include "custom_window.h"

class CSFrame : public CSWindow {
private:
	static bool class_registered;
	DYNCSTR frame_class = TEXT("CSFRAME_CLASS");

protected:
	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent);

public:
	CSFrame();

	CSFrame(HINSTANCE hInstance, PCSWindow parent);
};