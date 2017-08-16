#include <windows.h>

#include "resource.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwnd1, hwnd2, button, button2;

#define ADD_LINE_ID 1001
#define REMOVE_LINE_ID 1002

LPCSTR GetLastErrorAsString() {
	//Get the error message, if any.
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return ""; //No error message has been recorded

	LPSTR messageBuffer;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	LPCSTR message = messageBuffer;

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCMLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloApplication");
	static TCHAR szAppName2[] = TEXT("SineApplication");

	MSG msg;
	WNDCLASS mainwndclass, childwndclass;

	mainwndclass.style = CS_HREDRAW | CS_VREDRAW;
	mainwndclass.lpfnWndProc = MainWndProc;
	mainwndclass.cbClsExtra = 0;
	mainwndclass.cbWndExtra = sizeof(long);
	mainwndclass.hInstance = hInstance;
	mainwndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	mainwndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mainwndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mainwndclass.lpszMenuName = NULL;
	mainwndclass.lpszClassName = szAppName;

	HBRUSH bckgroundBrush = CreateSolidBrush(RGB(240, 255, 255));
	childwndclass.style = CS_HREDRAW | CS_VREDRAW;
	childwndclass.lpfnWndProc = ChildWndProc;
	childwndclass.cbClsExtra = 0;
	childwndclass.cbWndExtra = sizeof(long);
	childwndclass.hInstance = hInstance;
	childwndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	childwndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	childwndclass.hbrBackground = bckgroundBrush;
	childwndclass.lpszMenuName = NULL;
	childwndclass.lpszClassName = szAppName2;

	if (!RegisterClass(&mainwndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	if (!RegisterClass(&childwndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			szAppName2, MB_ICONERROR);
		return 0;
	}

	hwnd1 = CreateWindow(szAppName,
		TEXT("Hello World for Windows"),
		WS_OVERLAPPEDWINDOW /*& ~(WS_SIZEBOX)*/,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd1, iCmdShow);
	UpdateWindow(hwnd1);

	
	hwnd2 = CreateWindow(szAppName2,
		TEXT("Hello World"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		300,
		300,
		NULL,
		NULL,
		hInstance,
		NULL);
	//LPCSTR error = GetLastErrorAsString();
	//MessageBox(hwnd1, error, "Possible Error (Blank if No Error)", MB_OK);

	ShowWindow(hwnd2, iCmdShow);
	UpdateWindow(hwnd2);

	SetParent(hwnd2, hwnd1); //a will be the new parent b
	DWORD style = GetWindowLong(hwnd2, GWL_STYLE); //get the b style
	style &= ~(WS_POPUP | WS_CAPTION | WS_SIZEBOX); //reset the "caption" and "popup" bits
	style |= WS_CHILD; //set the "child" bit
	SetWindowLong(hwnd2, GWL_STYLE, style); //set the new style of b
	//RECT rc; //temporary rectangle
	//GetClientRect(hwnd1, &rc); //the "inside border" rectangle for a
	MoveWindow(hwnd2, 0, 0, 400, 400, TRUE); //place b at (x,y,w,h) in a

	UpdateWindow(hwnd1);
	UpdateWindow(hwnd2);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}



LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//HDC     hdc;
	//PAINTSTRUCT ps;
	//RECT    rect;

	switch (message)
	{
	case WM_CREATE:
		button = CreateWindow("BUTTON", "Attach new line", WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 50, 200, 20, hwnd, (HMENU)ADD_LINE_ID, NULL, NULL);
		//LPCSTR error = GetLastErrorAsString();
		//MessageBox(hwnd1, error, "Possible Error (Blank if No Error)", MB_OK);
		button2 = CreateWindow("BUTTON", "Remove last line", WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 75, 200, 20, hwnd, (HMENU)REMOVE_LINE_ID, NULL, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ADD_LINE_ID:
			//MessageBox(hwnd, "test", "test", MB_OK);
			PostMessage(hwnd2, ADD_LINE_ID, 0, 0);
			break;
		case REMOVE_LINE_ID:
			PostMessage(hwnd2, REMOVE_LINE_ID, 0, 0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


HBITMAP g_hbmBall;
#define PADDING 10

typedef struct Coord_Template {
	int x;
	int y;
} Coord;

typedef struct CoordList_Template {
	int size;
	Coord* coord_list;
} CoordList;
CoordList coord_list;


int get_random_pixel_coord(int window_dimension, int outer_padding) {
	/*given a window_dimension, return a value representing a pixel coord. from 0 to window_dimension
	use outer_padding to limit range (so placing a button for example isn't on total edge
	the minimum value will always be 0, no matter the outer_padding*/
	int target_range = window_dimension - outer_padding;
	if (target_range <= 0) {
		target_range = 1;
	}
	int res_coord = rand();
	res_coord %= target_range;

	return res_coord;
}

/*
Coord generate_random_coord(HWND hwnd) {
	Coord res;
	return res;
}*/

Coord generate_random_coord(HWND hwnd) {
	RECT rectHolder;
	GetClientRect(hwnd, &rectHolder);
	
	int res_x = get_random_pixel_coord(rectHolder.right, PADDING);
	int res_y = get_random_pixel_coord(rectHolder.bottom, PADDING);

	Coord res_coord;
	res_coord.x = res_x;
	res_coord.y = res_y;
	return res_coord;
}

void initialize_coord_list(CoordList* coord_list) {
	(*coord_list).size = 0;
	(*coord_list).coord_list = malloc(0);
}

void add_coord(CoordList* coord_list, Coord coord) {
	Coord* current_list = (*coord_list).coord_list; //get a copy to pointer to old coord_list
	int current_size = (*coord_list).size;

	(*coord_list).size++;
	int new_size = (*coord_list).size;

	(*coord_list).coord_list = malloc(new_size * sizeof(Coord));

	Coord* coord_list_ptr = (*coord_list).coord_list;
	int i;
	for (i = 0; i < current_size; i++) {
		*(coord_list_ptr + i) = *(current_list + i);
	}
	int new_size_index = new_size - 1;
	*(coord_list_ptr + new_size_index) = coord;

	free(current_list); //free allocated memory of old coord_list when done
}

void remove_last_coord(CoordList* coord_list) {
	Coord* current_list = (*coord_list).coord_list; //get a copy to pointer to old coord_list
	int current_size = (*coord_list).size;

	if (current_size != 0) {
		(*coord_list).size--;
		int new_size = (*coord_list).size;

		(*coord_list).coord_list = malloc(new_size * sizeof(Coord));

		Coord* coord_list_ptr = (*coord_list).coord_list;
		int i;
		for (i = 0; i < new_size; i++) {
			*(coord_list_ptr + i) = *(current_list + i);
		}
		//int new_size_index = new_size - 1;
		//*(coord_list_ptr + new_size_index) = coord;

		free(current_list); //free allocated memory of old coord_list when done
	}
}

void draw_coords(HDC current_dc, CoordList coord_list) {
	int list_size = coord_list.size;
	Coord* current_list = coord_list.coord_list;
	Coord current_coord;

	int i;
	for (i = 0; i < list_size; i++) {
		current_coord = *(current_list + i);
		LineTo(current_dc, current_coord.x, current_coord.y);
	}
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC     hdc;
	PAINTSTRUCT ps;
	RECT    rect;
	BITMAP bmp_data;
	RECT rect_holder;
	//HPEN pen = (HPEN) GetStockObject(BLACK_PEN);
	HPEN pen;

	switch (message) {
	case WM_CREATE:
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		g_hbmBall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
		if (g_hbmBall == NULL) {
			MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		initialize_coord_list(&coord_list);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		MoveToEx(hdc, 0, 0, NULL);

		HDC temp_mem_dc = CreateCompatibleDC(hdc);
		HBITMAP old_bitmap = SelectObject(temp_mem_dc, g_hbmBall);

		GetObject(g_hbmBall, sizeof(BITMAP), &bmp_data);
		BitBlt(hdc, 0, 0, bmp_data.bmWidth, bmp_data.bmHeight, temp_mem_dc, 0, 0, SRCCOPY);

		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Device Context"), -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		pen = CreatePen(PS_DASH, 1, RGB(0, 200, 100));
		HPEN old_pen = (HPEN) SelectObject(hdc, pen);
		draw_coords(hdc, coord_list);
		//LineTo(hdc, get_random_pixel_coord(rect.right, 10),
		//	get_random_pixel_coord(rect.bottom, 10));

		//LineTo(hdc, 100, 10);
		//LineTo(hdc, 10, 100);

		SelectObject(hdc, old_pen);
		SelectObject(temp_mem_dc, old_bitmap);
		DeleteDC(temp_mem_dc);

		EndPaint(hwnd, &ps);
		return 0;
	case ADD_LINE_ID:
		//MessageBox(hwnd, "In child window", "test", MB_OK);
		Coord rand_coord = generate_random_coord(hwnd);
		add_coord(&coord_list, rand_coord);
		//PostMessage(hwnd, WM_PRINT, (WPARAM)GetDC(hwnd), (LPARAM)PRF_CLIENT);
		GetClientRect(hwnd, &rect_holder);
		InvalidateRect(hwnd, &rect_holder, TRUE);
		return 0;
	case REMOVE_LINE_ID:
		remove_last_coord(&coord_list);
		GetClientRect(hwnd, &rect_holder);
		InvalidateRect(hwnd, &rect_holder, TRUE);
		return 0;
	case WM_DESTROY:
		DeleteObject(g_hbmBall);
		DeleteObject(pen);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

