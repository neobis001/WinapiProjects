#pragma once
#include "custom_frame.h"
#include "custom_edit.h"
#include "custom_updown.h"

#define BASE_UPDOWN_WIDTH 18 

class CSSpinbox;
typedef CSSpinbox* PCSSpinbox;

class CSSpinbox : public CSFrame {
private:
	bool is_setup = false;

	void get_modified_sb_dim(CSLOCRECT& edit, CSLOCRECT& spinbox);

protected:
	PCSWindow buddy = nullptr;
	PCSUpdown updown = nullptr;

	virtual void prg_draw_as_custom(HDWP* hdwp);

	virtual void prg_draw_children();

public:
	CSSpinbox();

	CSSpinbox(HINSTANCE hInstance, PCSWindow parent);

	void setup_sb_windows(PCSWindow buddy, PCSUpdown updown, int min, int max, int start);

	void set_spinbox_range(int min_value, int max_value);

	void set_spinbox_pos(int value); //will allow default handling for out-of-range values

	INT get_spinbox_pos();

};