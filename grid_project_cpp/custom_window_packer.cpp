#include "core_packer_algorithm.h"

void CSWindow::set_packing_cavity() {
	CSRECT res_cavity = calc_min_pack_cavity(children, num_children);

	pack_min_cavity_width = res_cavity.width;
	pack_min_cavity_height = res_cavity.height;

	if (resize_on_pack) {
		resize_via_packing_cavity();
	}
}

void CSWindow::prg_draw_as_pack(HDWP* hdwp) {
	CSRECT client_rect;
	client_rect.width = get_client_width();
	client_rect.height = get_client_height();
	CAVITY cavity_of_interest = create_default_cavity(client_rect);

	P2CSWindow processing_list = nullptr;
	int processing_list_size = 0;

	int current_start_index = 0;
	int current_end_index = 0;
	SIDE start_side = NULL_SIDE;

	while (current_end_index < num_children) { //keep track of --end-- index, not start
		PCSWindow window_of_interest = *(children + current_end_index);
		SIDE side_to_consider = window_of_interest->get_packing_side();

		if (processing_list == nullptr) {
			add_window(&processing_list, &processing_list_size, window_of_interest);
			start_side = side_to_consider;
			current_end_index++;

			if (current_end_index == num_children) {
				cout << "at end of window list (after first entry add), should process and reset list now " << endl;
				process_and_reset_windows(&processing_list, &processing_list_size, hdwp, &cavity_of_interest);
			}
		}
		else {
			if (start_side != NULL_SIDE) {
				if (is_equal_or_opp_side(start_side, side_to_consider)) {
					add_window(&processing_list, &processing_list_size, window_of_interest);
					current_end_index++;

					if (current_end_index == num_children) {
						cout << "at end of window list (after non-first entry add), should process and reset list now " << endl;
						process_and_reset_windows(&processing_list, &processing_list_size, hdwp, &cavity_of_interest);
					}
				}
				else {
					add_window(&processing_list, &processing_list_size, window_of_interest);
					process_and_reset_windows(&processing_list, &processing_list_size, hdwp, &cavity_of_interest);

					current_start_index = current_end_index;
					start_side = NULL_SIDE;
				}
			}
			else {
				cout << "start_side is NULL_SIDE, shouldn't be in this block" << endl;
			}
		}

	}

	if (processing_list != nullptr) {
		reset_list<P2CSWindow>(&processing_list, &processing_list_size);
	}
}