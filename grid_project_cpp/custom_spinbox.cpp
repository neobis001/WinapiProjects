#include "custom_spinbox.h"

void CSSpinbox::get_modified_sb_dim(CSLOCRECT& edit, CSLOCRECT& spinbox) {
	int	current_edit_width = get_client_width();
	int current_edit_height = get_client_height();

	int new_updown_width = get_min_value(BASE_UPDOWN_WIDTH, current_edit_width); //the new updown width could be
																				 //either it's original size. if the proposed edit width is actually so small it's smaller than the
																				 //updown width, then the updown width should be capped and fill this entire width space

	current_edit_width -= new_updown_width; //could've subtracted original_updown_width too
											//would just have to cut off width at 0 later on so that new_updown_x doesn't become negative
											//due to a negative current_edit_width becoming negative itself

											//note with using this new_updown_width, you're either
											//1. subtracting original_updown_width
											//2. subtracting current_edit_width itself (which would always lead to 0)
	current_edit_width = get_max_value(0, current_edit_width);
	int new_updown_x = current_edit_width;

	edit.loc.x = 0;
	edit.loc.y = 0;
	edit.dim.width = current_edit_width;
	edit.dim.height = current_edit_height;

	spinbox.loc.x = new_updown_x;
	spinbox.loc.y = 0;
	spinbox.dim.width = new_updown_width;
	spinbox.dim.height = current_edit_height;
}

void CSSpinbox::prg_draw_as_custom(HDWP* hdwp) {
	if (buddy == nullptr || updown == nullptr) {
		msg_force_quit("Error (in CSSpinbox::prg_draw_as_custom): either buddy or updown window is nullptr");
	}
	else {
		CSLOCRECT edit;
		CSLOCRECT sb;
		get_modified_sb_dim(edit, sb);
		buddy->prg_defer_window_pos(hdwp, NULL, edit.loc.x, edit.loc.y,
			edit.dim.width, edit.dim.height, 0);
		updown->prg_defer_window_pos(hdwp, NULL, sb.loc.x, sb.loc.y, sb.dim.width,
			sb.dim.height, 0);
	}
}

void CSSpinbox::prg_draw_children() {
	if (!is_setup) {
		msg_force_quit("Error (in CSSpinbox::prg_draw_children): spinbox is still not setup");
	}
	else {
		CSFrame::prg_draw_children();
	}
}

CSSpinbox::CSSpinbox() {

}

CSSpinbox::CSSpinbox(HINSTANCE hInstance, PCSWindow parent) {
	init_with_defaults(hInstance, parent);
}

void CSSpinbox::setup_sb_windows(PCSWindow buddy, PCSUpdown updown, int min, int max, int start) {
	this->buddy = buddy;
	this->updown = updown;

	buddy->request_custom_placing();
	updown->request_custom_placing();

	updown->set_range(min, max);
	updown->set_buddy(buddy);
	updown->set_pos(start);
	
	is_setup = true;
}

void CSSpinbox::set_spinbox_range(int min_value, int max_value) {
	updown->set_range(min_value, max_value);
}

void CSSpinbox::set_spinbox_pos(int value) {
	updown->set_pos(value);
}

INT CSSpinbox::get_spinbox_pos() {
	return updown->get_pos();
}