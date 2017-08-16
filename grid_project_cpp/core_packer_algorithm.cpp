//Windows API implementation of Tk's pack algorithm
//Tk's pack algorithm: https://www.tcl.tk/man/tcl8.5/TkCmd/pack.htm 
//completed working version: 7/20/17 
//by Nicolas Obias

//probably a lot I can clean up at this time, but still I'm extremely happy to have made this work
//took 1 week or more, pack algorithm was extremely difficult to implement, especially the minimal packing dimensions part
//a lot of frustrations and motivation issues, never worked on an algorithm this big before
//happy to see it all work in the end

//big idea: recursive pack calculation (if children haved packed_children_setting on, calc their pack first)

#include "core_packer_algorithm.h"

CSRECT calc_min_pack_cavity(PCSWindow* children, int size) {
	CSRECT current_cavity;
	current_cavity.width = -1; //set defaulted to error number
	current_cavity.height = -1;

	if (size < 0) {
		msg_force_quit("Error (in calc_min_pack): size input is less than 0");
	} else if (size == 0) {
		current_cavity.width = 0;
		current_cavity.height = 0;
	}
	else if (size == 1) {
		PCSWindow w = *(children + 0);
		current_cavity.width = w->get_base_width();
		current_cavity.height = w->get_base_height();
	}
	else {
		PCSWindow current_window = *(children + 0);

		//
		//first window processing
		SIDE requested_side = current_window->get_packing_side();
		current_cavity.width = current_window->get_base_width();
		current_cavity.height = current_window->get_base_height();

		CSRECT placing_rect = create_zeroed_struct<CSRECT>();

		switch (requested_side) {
		case LEFT: {
			placing_rect.width = 0;
			placing_rect.height = current_window->get_base_height();
			break;
		}
		case RIGHT: {
			placing_rect.width = 0;
			placing_rect.height = current_window->get_base_height();
			break;
		}
		case TOP: {
			placing_rect.height = 0;
			placing_rect.width = current_window->get_base_width();
			break;
		}
		case BOTTOM: {
			placing_rect.height = 0;
			placing_rect.width = current_window->get_base_width();
			break;
		}
		default: {
			cout << "Warning: in default" << endl;
			break;
		}
		}

		for (int i = 1; i < size; i++) {
			PCSWindow previous_window = *(children + (i - 1));
			current_window = *(children + i);
			requested_side = current_window->get_packing_side();

			//goal:
			//update cavity
			//update placing CSRECT
			//not concerned about resulting window dimensions
			if (requested_side == TOP || requested_side == BOTTOM) {
				if (current_window->get_base_width() > placing_rect.width) {
					current_cavity.width += current_window->get_base_width() - placing_rect.width;
					placing_rect.width = current_window->get_base_width();
				} //else change nothing

				if (current_window->get_base_height() > placing_rect.height) {
					current_cavity.height += current_window->get_base_height() - placing_rect.height;
					placing_rect.height = 0;
				}
				else if (current_window->get_base_height() == placing_rect.height) {
					placing_rect.height = 0;
				}
				else {
					placing_rect.height = placing_rect.height - current_window->get_base_height();
				}
			}
			else if (requested_side == LEFT || requested_side == RIGHT) {
				if (current_window->get_base_height() > placing_rect.height) {
					current_cavity.height += current_window->get_base_height() - placing_rect.height;
					placing_rect.height = current_window->get_base_height();
				}

				if (current_window->get_base_width() > placing_rect.width) {
					current_cavity.width += current_window->get_base_width() - placing_rect.width;
					placing_rect.width = 0;
				}
				else if (current_window->get_base_width() == placing_rect.width) {
					placing_rect.width = 0;
				}
				else {
					placing_rect.width = placing_rect.width - current_window->get_base_width();
				}
			}
		}
	}

	return current_cavity;
}

bool is_equal_or_opp_side(SIDE side_for_comparison, SIDE side_input) {
	if (side_for_comparison == TOP || side_for_comparison == BOTTOM) {
		if (side_input == TOP || side_input == BOTTOM) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (side_input == LEFT || side_input == RIGHT) {
			return true;
		}
		else {
			return false;
		}
	}
}

void add_window(P2CSWindow* windows, int* list_size, PCSWindow new_window) {
	add_list_entry<PCSWindow>(windows, list_size, new_window);
}

bool cavity_is_flat(CAVITY cavity) {
	if (cavity.current_cavity.width == 0 ||
		cavity.current_cavity.height == 0) {
		return true;
	}
	else {
		return false;
	}
}

void prepare_errored_pmap(PPARCELMAP parcel_map) {
	parcel_map->parcel_loc.x = -1; //negative numbers signify failed run
	parcel_map->parcel_loc.y = -1;
	parcel_map->parcel_rect.width = -1;
	parcel_map->parcel_rect.height = -1;
}

void draw_from_parcelmap(PPARCELMAP parcel_map) {
	PCSWindow window = parcel_map->window;
	HDWP* ptr_hdwp = parcel_map->hdwp;
	CSLOCRECT target_lrect = create_zeroed_struct<CSLOCRECT>();
	target_lrect.loc.x = parcel_map->parcel_loc.x;
	target_lrect.loc.y = parcel_map->parcel_loc.y;
	target_lrect.dim.width = parcel_map->parcel_rect.width;
	target_lrect.dim.height = parcel_map->parcel_rect.height;


	CSLOCRECT res_rect = window->get_padded_rect_info(target_lrect);
	window->prg_defer_window_pos(ptr_hdwp, NULL, res_rect.loc.x, res_rect.loc.y,
		res_rect.dim.width, res_rect.dim.height, 0);
}

void process_pmap_topsided(PPARCELMAP parcel_map, int requested_height, PCAVITY remaining_cavity) {
	PPARCELMAP pmap = parcel_map;
	PCAVITY rc = remaining_cavity;

	pmap->parcel_loc = rc->current_point;
	pmap->parcel_rect.width = rc->current_cavity.width;

	if (requested_height >= rc->current_cavity.height) {
		pmap->parcel_rect.height = rc->current_cavity.height;
		rc->current_point.y += rc->current_cavity.height;
		rc->current_cavity.height = 0;
	}
	else {
		pmap->parcel_rect.height = requested_height;
		rc->current_point.y += requested_height;
		rc->current_cavity.height = rc->current_cavity.height - requested_height;
	}
}

void process_parcelmap_bottomsided(PPARCELMAP parcel_map, int requested_height, PCAVITY remaining_cavity) {
	PPARCELMAP pmap = parcel_map;
	PCAVITY rc = remaining_cavity;

	pmap->parcel_rect.width = rc->current_cavity.width;

	if (requested_height >= rc->current_cavity.height) {
		pmap->parcel_rect.height = rc->current_cavity.height;
		pmap->parcel_loc = rc->current_point;
		//current_point.y doesn't change
		rc->current_cavity.height = 0;
	}
	else {
		pmap->parcel_rect.height = requested_height;
		pmap->parcel_loc.x = rc->current_point.x;
		pmap->parcel_loc.y = rc->current_point.y + (rc->current_cavity.height - requested_height);
		//current_point.y doesn't change
		rc->current_cavity.height = rc->current_cavity.height - requested_height;
	}
}

void process_pmap_leftsided(PPARCELMAP parcel_map, int requested_width, PCAVITY remaining_cavity) {
	PPARCELMAP pmap = parcel_map;
	PCAVITY rc = remaining_cavity;

	pmap->parcel_loc = rc->current_point;
	pmap->parcel_rect.height = rc->current_cavity.height;

	if (requested_width >= rc->current_cavity.width) {
		pmap->parcel_rect.width = rc->current_cavity.width;
		rc->current_point.x += rc->current_cavity.width;
		rc->current_cavity.width = 0;
	}
	else {
		pmap->parcel_rect.width = requested_width;
		rc->current_point.x += requested_width;
		rc->current_cavity.width = rc->current_cavity.width - requested_width;
	}
}

void process_parcelmap_rightsided(PPARCELMAP parcel_map, int requested_width, PCAVITY remaining_cavity) {
	PPARCELMAP pmap = parcel_map;
	PCAVITY rc = remaining_cavity;

	pmap->parcel_rect.height = rc->current_cavity.height;

	if (requested_width >= rc->current_cavity.width) {
		pmap->parcel_rect.width = rc->current_cavity.width;
		pmap->parcel_loc = rc->current_point;
		//current_point.x doesn't change
		rc->current_cavity.width = 0;
	}
	else {
		pmap->parcel_rect.width = requested_width;
		pmap->parcel_loc.x = rc->current_point.x + (rc->current_cavity.width - requested_width);
		pmap->parcel_loc.y = rc->current_point.y;
		//current_point.x doesn't change
		rc->current_cavity.width = rc->current_cavity.width - requested_width;
	}
}

void process_parcelmap(PPARCELMAP parcel_map, CSRECT requested_parcel, PCAVITY remaining_cavity) {
	PPARCELMAP pmap = parcel_map; //so it's easier to type
	PCAVITY rc = remaining_cavity;

	if (cavity_is_flat(*remaining_cavity)) {
		cout << "(MESSAGE START)" << endl;
		cout << "Warning (in process_parcelmap): remaining_cavity has either width 0 or height 0," <<
			" so will not do anything" << endl;
		//cout << "window id being considered: " << pmap->window.window_num << endl;
		cout << "remaining_cavity width: " << rc->current_cavity.width << endl;
		cout << "remaining_cavity height: " << rc->current_cavity.height << endl;
		cout << "(MESSAGE END)" << endl;
		return;
	}
	else {
		SIDE requested_side = pmap->window->get_packing_side();
		int requested_height = requested_parcel.height;
		int requested_width = requested_parcel.width;
		prepare_errored_pmap(pmap);

		if (requested_side == TOP) {
			process_pmap_topsided(parcel_map, requested_height, remaining_cavity);
		}
		else if (requested_side == BOTTOM) {
			process_parcelmap_bottomsided(parcel_map, requested_height, remaining_cavity);
		}
		else if (requested_side == LEFT) {
			process_pmap_leftsided(parcel_map, requested_width, remaining_cavity);
		}
		else if (requested_side == RIGHT) {
			process_parcelmap_rightsided(parcel_map, requested_width, remaining_cavity);
		}

		draw_from_parcelmap(parcel_map);
	}
}

int get_combined_window_dimemsions(P2CSWindow windows, int list_size, DIMENSION dimension) {
	int total_dimension = 0;

	if (dimension == WIDTH) {
		for (int i = 0; i < list_size; i++) {
			PCSWindow window = *(windows + i);
			total_dimension += window->get_base_width();
		}
	}
	else {
		for (int i = 0; i < list_size; i++) {
			PCSWindow window = *(windows + i);
			total_dimension += window->get_base_height();
		}
	}

	return total_dimension;
}

bool any_window_has_expand(P2CSWindow window, int list_size) {
	bool res = false;
	for (int i = 0; i < list_size; i++) {
		PCSWindow window_of_interest = *(window + i);
		if (window_of_interest->get_packing_expand() == true) {
			res = true;
			break;
		}
	}
	return res;
}

void get_pmaps_via_expansion(P2PARCELRECT pmap_list, int list_size, bool find_pmaps_with_expand,
	P2PARCELRECT* res_list, int* res_size) {

	*res_list = nullptr;
	*res_size = 0;

	if (find_pmaps_with_expand) {
		for (int i = 0; i < list_size; i++) {
			PPARCELRECT prect = *(pmap_list + i);
			PCSWindow w = prect->pmap.window;
			if (w->get_packing_expand() == true) {
				add_list_entry<PPARCELRECT>(res_list, res_size, prect);
			}
		}
	}
	else {
		for (int i = 0; i < list_size; i++) {
			PPARCELRECT prect = *(pmap_list + i);
			PCSWindow w = prect->pmap.window;
			if (w->get_packing_expand() == false) {
				add_list_entry<PPARCELRECT>(res_list, res_size, prect);
			}
		}
	}
}

CSRECT get_normal_parcel(PCSWindow window, PCAVITY remaining_cavity) {
	CSRECT requested_parcel = create_zeroed_struct<CSRECT>();

	if (window->get_packing_side() == TOP || window->get_packing_side() == BOTTOM) {
		requested_parcel.width = remaining_cavity->current_cavity.width;
		requested_parcel.height = window->get_base_height();
	}
	else { 		//process as it was LEFT || RIGHT
		requested_parcel.height = remaining_cavity->current_cavity.height;
		requested_parcel.width = window->get_base_width();
	}

	return requested_parcel;
}

CSRECT get_expanded_parcel(PCSWindow window, PCAVITY remaining_cavity) {
	CSRECT requested_parcel = create_zeroed_struct<CSRECT>();

	if (window->get_packing_expand() == true) {
		requested_parcel = remaining_cavity->current_cavity;
	}
	else {
		requested_parcel = get_normal_parcel(window, remaining_cavity);
	}

	return requested_parcel;
}

void process_nexpand_windows(P2CSWindow windows, int list_size, HDWP* hdwp,
	PCAVITY remaining_cavity) {
	for (int i = 0; i < list_size; i++) {
		if (cavity_is_flat(*remaining_cavity)) {
			break;
		}
		else {
			PCSWindow window_of_interest = *(windows + i);

			PARCELMAP pmap = create_zeroed_struct<PARCELMAP>();
			pmap.window = window_of_interest;
			pmap.hdwp = hdwp;

			CSRECT requested_parcel = get_normal_parcel(window_of_interest, remaining_cavity);

			process_parcelmap(&pmap, requested_parcel, remaining_cavity);
		}
	}
}

void process_windows_via_ends_normally(P2CSWindow windows, int list_size, HDWP* hdwp, PCAVITY remaining_cavity,
	SIDE first_window_side, SIDE last_window_side) {
	if (is_equal_or_opp_side(first_window_side, last_window_side)) {
		process_nexpand_windows(windows, list_size, hdwp, remaining_cavity);
	}
	else {
		int input_list_size = list_size - 1;
		P2CSWindow input_windows = new PCSWindow[input_list_size];
		for (int i = 0; i < input_list_size; i++) {
			*(input_windows + i) = *(windows + i);
		}

		process_nexpand_windows(input_windows, input_list_size, hdwp, remaining_cavity);
	}
}

void process_windows(P2CSWindow windows, int list_size, HDWP* hdwp, PCAVITY remaining_cavity) { //don't think will need start side
																					//can infer that from 1st window
	if (cavity_is_flat(*remaining_cavity)) {
		return; //there are cases where user can reasonably flatten window, so don't send message 
		  //in this case
	}
	else if (list_size < 1) { //should always be at least one, if used in get_packing_results i think
		DYNCHAR buffer[200];
		wsprintf(buffer, "error (in process_windows): list size should be at least 1 (inputted %i)",
			list_size);
		msg_force_quit(buffer);
	}
	else if (list_size == 1) {
		PCSWindow window_of_interest = *(windows + 0);

		PARCELMAP pmap = create_zeroed_struct<PARCELMAP>();
		pmap.window = window_of_interest;
		pmap.hdwp = hdwp;

		CSRECT requested_parcel = get_expanded_parcel(window_of_interest, remaining_cavity);

		process_parcelmap(&pmap, requested_parcel, remaining_cavity);
	}
	else {

		PCSWindow first_window = *(windows + 0);
		PCSWindow last_window = *(windows + (list_size - 1));
		SIDE first_window_side = first_window->get_packing_side();
		SIDE last_window_side = last_window->get_packing_side();


		if (first_window_side == TOP || first_window_side == BOTTOM) {
			int combined_window_height = get_combined_window_dimemsions(windows, list_size, HEIGHT);

			if (combined_window_height < remaining_cavity->current_cavity.height) {
				process_windows_via_ends(windows, list_size, hdwp, remaining_cavity,
					first_window_side, last_window_side, HEIGHT);
			}
			else {
				process_windows_via_ends_normally(windows, list_size, hdwp, remaining_cavity,
					first_window_side, last_window_side);
			}
		}
		else {
			int combined_window_width = get_combined_window_dimemsions(windows, list_size, WIDTH);

			if (combined_window_width < remaining_cavity->current_cavity.width) {
				process_windows_via_ends(windows, list_size, hdwp, remaining_cavity,
					first_window_side, last_window_side, WIDTH);
			}
			else {
				process_windows_via_ends_normally(windows, list_size, hdwp, remaining_cavity,
					first_window_side, last_window_side);
			}
		}
	}
}

CAVITY create_default_cavity(CSRECT window_rect) {
	CAVITY res_cavity = create_zeroed_struct<CAVITY>(); //should use copy of cavity to not override the one passed in
	res_cavity.current_cavity = window_rect;
	res_cavity.current_point = { 0, 0 };
	return res_cavity;
}

void process_and_reset_windows(P2CSWindow* processing_list, int* list_size, HDWP* hdwp, PCAVITY cavity_of_interest) {
	process_windows(*processing_list, *list_size, hdwp, cavity_of_interest); //don't need to mutate
																	   //processing_list, but need to mutate cavity of interest w/ each while iteration

	reset_list<P2CSWindow>(processing_list, list_size);
}

void process_expand_windows(P2CSWindow windows, int list_size, HDWP* hdwp,
	int available_expanding_space, PCAVITY remaining_cavity, DIMENSION ref_dimension) {
	P2PARCELRECT prects = new PPARCELRECT[list_size]; //can't do non-new list, won't accept const int 
	int prects_size = list_size;
	//if initialized to changing variable
	for (int i = 0; i < prects_size; i++) {
		PPARCELRECT prect = create_zeroed_single_ptr<PARCELRECT>();
		prect->pmap.window = *(windows + i);
		prect->pmap.hdwp = hdwp;
		*(prects + i) = prect;
	}

	P2PARCELRECT non_expand_prects = nullptr;
	int nexpand_list_size = 0;
	get_pmaps_via_expansion(prects, prects_size, false,
		&non_expand_prects, &nexpand_list_size);


	P2PARCELRECT expand_pmaps = nullptr;
	int expand_list_size = 0;
	get_pmaps_via_expansion(prects, prects_size, true, &expand_pmaps, &expand_list_size);

	for (int i = 0; i < nexpand_list_size; i++) {
		PPARCELRECT ptr_prect = *(non_expand_prects + i);
		int base_dimension = get_prect_base_dim(ptr_prect, ref_dimension);
		modify_rect_in_parcelrect(ptr_prect, ref_dimension, base_dimension);
		available_expanding_space -= base_dimension;

		if (available_expanding_space < 0) {
			available_expanding_space = 0;
			msg_force_quit("warning (in process_expand_windows): available_expanding_space went lower than 0"\
				"though never should've.");
		}
	}

	int divided_expanding_space = available_expanding_space / expand_list_size;
	int leftover_expanding_space = available_expanding_space % expand_list_size;
	int last_expand_pmap_index = expand_list_size - 1;

	for (int i = 0; i < expand_list_size - 1; i++) {
		PPARCELRECT ptr_prect = *(expand_pmaps + i);
		modify_rect_in_parcelrect(ptr_prect, ref_dimension, divided_expanding_space);
	}
	PPARCELRECT last_expand_pmap = *(expand_pmaps + last_expand_pmap_index);
	int total_expanding_space = divided_expanding_space + leftover_expanding_space;
	modify_rect_in_parcelrect(last_expand_pmap, ref_dimension, total_expanding_space);

	for (int i = 0; i < prects_size; i++) {
		PPARCELRECT ptr_prect = *(prects + i);
		PARCELMAP pmap = ptr_prect->pmap;
		CSRECT requested_rect = ptr_prect->requested_rect;

		process_parcelmap(&pmap, requested_rect, remaining_cavity);
	}

	for (int i = 0; i < prects_size; i++) {
		PPARCELRECT ptr_prect = *(prects + i);
		delete ptr_prect;
	}
	delete[] prects;
	delete[] non_expand_prects; //with prects full deleted, don't need to consider pointers
								//these two lists are storing, so can just delete arrays by themselves
	delete[] expand_pmaps;
}

void modify_rect_in_parcelrect(PPARCELRECT ptr_prect, DIMENSION dimension, int new_dimension) {
	if (dimension == WIDTH) {
		ptr_prect->requested_rect.width = new_dimension;
	}
	else {
		ptr_prect->requested_rect.height = new_dimension;
	}
}

int get_prect_base_dim(PPARCELRECT ptr_prect, DIMENSION dimension) {
	if (dimension == WIDTH) {
		return ptr_prect->pmap.window->get_base_width();
	}
	else {
		return ptr_prect->pmap.window->get_base_height();
	}
}

void process_windows_via_side(P2CSWindow windows, int list_size, int available_expanding_space, HDWP* hdwp,
	PCAVITY remaining_cavity, DIMENSION dimension) {
	if (any_window_has_expand(windows, list_size)) {
		process_expand_windows(windows, list_size, hdwp, available_expanding_space,
			remaining_cavity, dimension);
	}
	else {
		process_nexpand_windows(windows, list_size, hdwp, remaining_cavity);
	}
}


void process_windows_via_ends(P2CSWindow windows, int list_size, HDWP* hdwp, PCAVITY remaining_cavity,
	SIDE first_window_side, SIDE last_window_side, DIMENSION dimension) {
	int available_expanding_space = 0; //only used
									//if at least one of the windows has expand option
	if (dimension == WIDTH) {
		available_expanding_space = remaining_cavity->current_cavity.width;
	}
	else {
		available_expanding_space = remaining_cavity->current_cavity.height;
	}


	if (is_equal_or_opp_side(first_window_side, last_window_side)) {
		//all windows must be similar, side-wise
		//after this, there should be no more window sets to consider

		process_windows_via_side(windows, list_size,
			available_expanding_space, hdwp, remaining_cavity, dimension);
	}
	else {
		//last window must be not similar to others, side-wise
		//pack all normally, --except-- last window (consider last window space)
		int input_list_size = list_size - 1;
		P2CSWindow input_windows = new PCSWindow[input_list_size];
		for (int i = 0; i < input_list_size; i++) {
			*(input_windows + i) = *(windows + i);
		}

		int last_window_index = list_size - 1;
		PCSWindow last_window = *(windows + last_window_index);
		
		if (dimension == WIDTH) {
			available_expanding_space -= last_window->get_base_width();
		}
		else {
			available_expanding_space -= last_window->get_base_height();
		}

		process_windows_via_side(input_windows, input_list_size, available_expanding_space, hdwp,
			remaining_cavity, dimension);
	}
}
