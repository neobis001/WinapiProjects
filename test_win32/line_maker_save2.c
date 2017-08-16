#include <windows.h>
#include <FreeImage.h>
#include "resource.h"

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwnd1, hwnd2, button, button2, button3;

#define ADD_LINE_ID 1001
#define REMOVE_LINE_ID 1002
#define SAVE_ID 1003

LPCSTR GetLastErrorAsString() {
	//Get the error message, if any.
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return TEXT(""); //No error message has been recorded

	LPSTR messageBuffer;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPSTR)&messageBuffer, 255, NULL);

	return (LPCSTR)messageBuffer;
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
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400,
		400,
		hwnd1,
		NULL,
		hInstance,
		NULL);

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
		button = CreateWindow(TEXT("BUTTON"), TEXT("Attach new line"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 50, 200, 20, hwnd, (HMENU)ADD_LINE_ID, NULL, NULL);
		//wchar_t error[256];
		//memset(error, 0, 256);
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
		//	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error, 255, NULL);
		button2 = CreateWindow(TEXT("Button"), TEXT("Remove last line"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 75, 200, 20, hwnd, (HMENU)REMOVE_LINE_ID, NULL, NULL);
		button3 = CreateWindow(TEXT("Button"), TEXT("Save Image"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 100, 200, 20, hwnd, (HMENU)SAVE_ID, NULL, NULL);
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
		case SAVE_ID:
			PostMessage(hwnd2, SAVE_ID, 0, 0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


HBITMAP g_hbmBall, g_bitmap;
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

INT a = 110;
INT b = 110;

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC     hdc;
	PAINTSTRUCT ps;
	RECT    rect;
	BITMAP bmp_data;
	RECT rect_holder;
	//HPEN pen = (HPEN) GetStockObject(BLACK_PEN);
	HPEN pen;
	Coord rand_coord;

	switch (message) {
	case WM_CREATE:
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		g_hbmBall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
		if (g_hbmBall == NULL) {
			MessageBox(hwnd, TEXT("Could not load IDB_BALL!"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
		}

		g_bitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TESTBMP));
		if (g_bitmap == NULL) {
			MessageBox(hwnd, TEXT("Could not load IDB_TESTBMP!"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
		}

		initialize_coord_list(&coord_list);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		/*
		MoveToEx(hdc, 0, 0, NULL);

		HDC temp_mem_dc = CreateCompatibleDC(hdc);
		HBITMAP old_bitmap = SelectObject(temp_mem_dc, g_bitmap);

		HRGN hrgn = CreateRectRgn(100, 100,
			300, 300);
		#define size 5
		POINT points[size] = { {0,0}, {100,0}, {85, 100}, {100, 200}, {0, 200} };
		HRGN hrgn2 = CreatePolygonRgn(points, size, ALTERNATE);
		HRGN res_rgn = CreateRectRgn(0,0, 10, 10);
		int res = CombineRgn(res_rgn, hrgn, hrgn2, RGN_OR);
		if (res == NULLREGION || res == ERROR) {
			MessageBox(NULL, "error", "", MB_OK);
		}
		SelectClipRgn(hdc, res_rgn);


		//SelectObject(hdc, old_pen);
		//SelectObject(temp_mem_dc, old_bitmap);
		//DeleteDC(temp_mem_dc);
		*/
		
		HPEN hPen = CreatePen(PS_SOLID, 13, RGB(100, 0, 0));

		HPEN oldPen = SelectObject(hdc, hPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));

		BeginPath(hdc);
		MoveToEx(hdc, 50, 50, NULL);

		LineTo(hdc, 150, 50);
		LineTo(hdc, 150, 150);

		POINT pts[3] = { {250, 175}, {225, 160}, {140,230}};
		PolyBezierTo(hdc, pts, 3);

		//CloseFigure(hdc);
		EndPath(hdc);

		SelectClipPath(hdc, RGN_COPY);


		HDC temp_dc = CreateCompatibleDC(hdc);
		HBITMAP old_bitmap = SelectObject(temp_dc, g_bitmap);
		BitBlt(hdc, 0, 0, 400, 400, temp_dc, 0, 0, SRCCOPY);

		DeleteDC(temp_dc);
		/*
		HBRUSH hOldBrush;
		HPEN   hOldPen, hPen;

		// Create a wide pen.
		//...................
		hPen = CreatePen(PS_SOLID, 8, RGB(0, 0, 0));

		hOldPen = SelectObject(hdc, hPen);
		hOldBrush = SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));

		// Create a path.
		//...............
		BeginPath(hdc);
		MoveToEx(hdc, 10, 10, NULL);
		LineTo(hdc, 110, 10);
		LineTo(hdc, 110, 110);
		CloseFigure(hdc);
		EndPath(hdc);


		// Redefine the path as the width of 
		// the pen surrounding the path.
		//..................................
		WidenPath(hdc);
		SelectObject(hdc, hOldPen);

		// Stroke and fill the path.
		//..........................
		StrokeAndFillPath(hdc);

		DeleteObject(hPen);
		*/

		EndPaint(hwnd, &ps);
		

		return 0;
	case ADD_LINE_ID:
		//MessageBox(hwnd, "In child window", "test", MB_OK);
		rand_coord = generate_random_coord(hwnd);
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
	case SAVE_ID:
		RECT testRect;
		GetClientRect(hwnd, &testRect);

		HDC window_dc = GetDC(hwnd);
		HDC compat_dc = CreateCompatibleDC(window_dc);

		HBITMAP hbmp = CreateCompatibleBitmap(window_dc, testRect.right, testRect.bottom);
		SelectObject(compat_dc, hbmp);

		BitBlt(compat_dc, 0, 0, testRect.right, testRect.bottom, window_dc, 0, 0, SRCCOPY);

		
		//Delete bitmap later?	
		DeleteDC(compat_dc);
		ReleaseDC(hwnd, window_dc);

		// ...
		// the following code assumes that you have a valid HBITMAP loaded into the memory
		FIBITMAP *dib = NULL;
		if (hbmp) {
			BITMAP bm;
			GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
			dib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, 0, 0, 0);
			// The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why) 
			// So we save these infos below. This is needed for palettized images only. 
			int nColors = FreeImage_GetColorsUsed(dib);
			HDC dc = GetDC(NULL);
			int Success = GetDIBits(dc, hbmp, 0, FreeImage_GetHeight(dib),
				FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
			ReleaseDC(NULL, dc);
			// restore BITMAPINFO members
			FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
			FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;
		}
		// ...
		// don't forget to call FreeImage_Unload when you no longer need the dib
		FreeImage_Save(FIF_BMP, dib, "test_image.bmp", 0);
		FreeImage_Unload(dib);

		//HDC mem_dc = CreateCompatibleDC()
		return 0;
	case WM_DESTROY:
		DeleteObject(g_hbmBall);
		DeleteObject(pen);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

