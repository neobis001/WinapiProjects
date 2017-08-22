#pragma once
#include "custom_window_types.h"

#include <CommCtrl.h>
#pragma comment(linker,"\"/manifestdependency:type                  = 'win32' \
                                              name                  = 'Microsoft.Windows.Common-Controls' \
                                              version               = '6.0.0.0' \
                                              processorArchitecture = '*' \
                                              publicKeyToken        = '6595b64144ccf1df' \
                                              language              = '*'\"")

void initialize_common_controls(INT class_ids);

void msg_light_quit(HWND hwnd, DYNCSTR info);

//display a message before doing a force quit 
//(sort of like End Process in Task Manager, I'm guessing)
void msg_force_quit(DYNCSTR info);

void msg_string(DYNCSTR info);

void msg_here(DYNCSTR extra_info = TEXT("None"));

//display a message box showing the last error that occurred
//if there's none, the message box will be blank
//note though that error model is vague (some errors showing aren't actually errors)
void msg_last_error();

void msg_int(int integer, DYNCSTR extra_info = TEXT("None"));

void msg_two_ints(int integer1, int integer2, DYNCSTR extra_info = TEXT("None"));

LPCSTR GetLastErrorAsString();

int get_min_value(int value1, int value2);

int get_max_value(int value1, int value2);

template <class T>
void msg_address(T* pointer) {
	DYNCHAR buffer[100];
	wsprintf(buffer, "pointer value: %p", pointer);
	msg_string(buffer);
}

template <class T>
bool in(T input, T* input_list, int list_size) {
	bool val_is_included = false;
	for (int i = 0; i < list_size; i++) {
		T comparing_value = *(input_list + i);
		if (comparing_value == input) {
			val_is_included = true;
			break;
		}
	}

	return val_is_included;
}

template <class T>
T* get_list_copy(T* list, int list_size) {
	if (list == nullptr) {
		return nullptr;
	}
	else {
		T* new_list = new T[list_size];

		for (int i = 0; i < list_size; i++) {
			*(new_list + i) = *(list + i);
		}
		return new_list;
	}
}

template <class T>
T create_zeroed_struct() { //assumes T is a struct
						   //note zeroing a class instance will zero out --all-- member variables
	T res_struct;
	memset(&res_struct, 0, sizeof(T));
	return res_struct;
}