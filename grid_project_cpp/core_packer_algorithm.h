#pragma once
#include "custom_window.h"

#include <iostream>
#include <cstring> //for memset

using namespace std;

enum DIMENSION {
	WIDTH,
	HEIGHT
};

typedef SIDE* PSIDE;

typedef PCSWindow* P2CSWindow;

typedef struct CAVITY_Template {
	CSPOINT current_point;
	CSRECT current_cavity;
} CAVITY;

typedef CAVITY* PCAVITY;

typedef struct PARCELMAP_Template {
	PCSWindow window;
	CSPOINT parcel_loc;
	CSRECT parcel_rect;
	HDWP* hdwp;
} PARCELMAP;

typedef PARCELMAP* PPARCELMAP;

typedef struct PARCELRECT_Template {
	PARCELMAP pmap;
	CSRECT requested_rect;
} PARCELRECT;

typedef PARCELRECT* PPARCELRECT;
typedef PPARCELRECT* P2PARCELRECT;

CSRECT calc_min_pack_cavity(PCSWindow* children, int size);

bool is_equal_or_opp_side(SIDE side_for_comparison, SIDE side_input);

void add_window(P2CSWindow* windows, int* list_size, PCSWindow new_window);

bool cavity_is_flat(CAVITY cavity);

void prepare_errored_pmap(PPARCELMAP parcel_map);

void draw_from_parcelmap(PPARCELMAP parcel_map);

void process_pmap_topsided(PPARCELMAP parcel_map, int requested_height, PCAVITY remaining_cavity);

void process_parcelmap_bottomsided(PPARCELMAP parcel_map, int requested_height, PCAVITY remaining_cavity);

void process_pmap_leftsided(PPARCELMAP parcel_map, int requested_width, PCAVITY remaining_cavity);

void process_parcelmap_rightsided(PPARCELMAP parcel_map, int requested_width, PCAVITY remaining_cavity);

void process_parcelmap(PPARCELMAP parcel_map, CSRECT requested_parcel, PCAVITY remaining_cavity);

int get_combined_window_dimemsions(P2CSWindow windows, int list_size, DIMENSION dimension);

bool any_window_has_expand(P2CSWindow window, int list_size);

CSRECT get_normal_parcel(PCSWindow window, PCAVITY remaining_cavity);

CSRECT get_expanded_parcel(PCSWindow window, PCAVITY remaining_cavity);

void process_windows(P2CSWindow windows, int list_size, HDWP* hdwp, PCAVITY remaining_cavity);

CAVITY create_default_cavity(CSRECT window_rect);

void process_and_reset_windows(P2CSWindow* processing_list, int* list_size, HDWP* hdwp, PCAVITY cavity_of_interest);

void process_expand_windows(P2CSWindow windows, int list_size, HDWP* hdwp,
	int available_expanding_space, PCAVITY remaining_cavity, DIMENSION ref_dimension);

void modify_rect_in_parcelrect(PPARCELRECT ptr_prect, DIMENSION dimension, int new_dimension);

int get_prect_base_dim(PPARCELRECT ptr_prect, DIMENSION dimension);

void process_windows_via_side(P2CSWindow windows, int list_size, int available_expanding_space, HDWP* hdwp,
	PCAVITY remaining_cavity, DIMENSION dimension);

void process_windows_via_ends(P2CSWindow windows, int list_size, HDWP* hdwp, PCAVITY remaining_cavity,
	SIDE first_window_side, SIDE last_window_side, DIMENSION dimension);

template <class T>
void add_list_entry(T** list, int* list_size, T entry) {
	int current_size = *list_size;
	*list_size = *(list_size)+1;
	int new_size = *list_size;

	T* current_list = *list;
	*list = new T[new_size];

	for (int i = 0; i < current_size; i++) {
		*(*list + i) = *(current_list + i);
	}
	int new_entry_index = new_size - 1;
	*(*list + new_entry_index) = entry;


	if (current_list != nullptr) {
		delete[] current_list;
		current_list = nullptr;
	}
}

template <class T>
void reset_list(T* list, int* list_size) {
	T actual_list = *list;
	delete[] actual_list;
	*list = nullptr;
	*list_size = 0;
}

template <class T>
T* create_zeroed_single_ptr() { //note: because of new, have to delete manually later
								//also, setting to nullptr is completely different, but still something to consider
								//also, note zeroing class instance zeros out --all-- members
	T* res_ptr = new T;
	memset(res_ptr, 0, sizeof(T));
	return res_ptr;
}
