#include "general_window_tools.h"

void initialize_common_controls(INT class_ids) {
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = class_ids;
	BOOL init_res = InitCommonControlsEx(&iccx);
	if (init_res == FALSE) {
		msg_string(TEXT("Warning(in initialize_common_controls) : InitCommonControlsEx failed"));
	}
}

void msg_light_quit(HWND hwnd, DYNCSTR info) {
	msg_string(info);
	PostMessage(hwnd, WM_CLOSE, 0, 0);
}

void msg_force_quit(DYNCSTR info) {
	msg_string(info);
	ExitProcess(0);
}

void msg_string(DYNCSTR info) {
	MessageBox(NULL, info, "No-arg Message", MB_OK);
}

void msg_here(DYNCSTR extra_info) {
	DYNCHAR buffer[500];
	wsprintf(buffer, "here, extra info: %s", extra_info);
	msg_string(buffer);
}

void msg_last_error() {
	DYNCSTR error_str = GetLastErrorAsString();
	MessageBox(NULL, error_str, "Error Message", MB_OK);
}

void msg_int(int integer, DYNCSTR extra_info) {
	DYNCHAR buffer[500];
	wsprintf(buffer, "integer value: %d\nextra info: %s", integer, extra_info);
	msg_string(buffer);
}

void msg_two_ints(int integer1, int integer2, DYNCSTR extra_info) {
	DYNCHAR buffer[500];
	wsprintf(buffer, "integer1 value: %d\ninteger 2 value: %d"\
		"\nextra info: %s", integer1, integer2, extra_info);
	msg_string(buffer);
}

DYNCSTR GetLastErrorAsString() { //most code from Stack Overflow
	//Get the error message, if any.
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return TEXT(""); //No error message has been recorded

	DYNSTR messageBuffer;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (DYNSTR)&messageBuffer, 255, NULL);

	return (DYNCSTR)messageBuffer;
}

int get_min_value(int value1, int value2) {
	if (value1 <= value2) {
		return value1;
	}
	else {
		return value2;
	}
}

int get_max_value(int value1, int value2) {
	if (value1 >= value2) {
		return value1;
	}
	else {
		return value2;
	}
}
