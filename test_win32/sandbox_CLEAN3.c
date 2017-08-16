#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

LPCSTR GetLastErrorAsString() {
	//Get the error message, if any.
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return TEXT(""); //No error message has been recorded

	LPSTR messageBuffer;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 255, NULL);

	//LPCWSTR message = messageBuffer;

	//Free the buffer.
	//LocalFree(messageBuffer);

	return (LPCSTR)messageBuffer;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		DestroyWindow(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow)
{
	
	AllocConsole();

	FILE *fpstdin = stdin, *fpstdout = stdout, *fpstderr = stderr;

	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);

	printf("Input a value.\n");
	int val = 0;
	scanf_s("%i", &val);
	printf("The value inputted was %i", val);

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	if (FreeConsole() == 0) {
		MessageBox(NULL, "FreeConsole failed", "", MB_OK);
	}

	AllocConsole();

	fpstdin = stdin; 
	fpstdout = stdout; 
	fpstderr = stderr;

	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);

	printf("Input a value.\n");
	val = 0;
	scanf_s("%i", &val);
	printf("The value inputted was %i", val);

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	if (FreeConsole() == 0) {
		MessageBox(NULL, "FreeConsole failed", "", MB_OK);
	}


	MSG Msg;
	WNDCLASSEX wc;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "TEST_CLASS";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"TEST_CLASS",
		"The title of my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 200,
		NULL, NULL, hInstance, NULL);

	if (hwnd == (void*)0)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0) 
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}



/*
HWND hwnd1, hwnd2, button, button2;

#define ADD_LINE_ID 1001
#define REMOVE_LINE_ID 1002
#define TIMER_ID 1003

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//HDC     hdc;
	//PAINTSTRUCT ps;
	//RECT    rect;

	switch (message)
	{
	case WM_CREATE:
		button = CreateWindow(TEXT("BUTTON"), TEXT("Attach new line"), WS_VISIBLE | WS_CHILD | WS_BORDER,
			400, 50, 200, 20, hwnd, (HMENU)ADD_LINE_ID, NULL, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ADD_LINE_ID:
			//MessageBox(hwnd, "test", "test", MB_OK);
			//PostMessage(hwnd2, ADD_LINE_ID, 0, 0);
			printf("The number is %i", 243);
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



int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG Msg;

	WNDCLASS mainwndclass;

	mainwndclass.style = CS_HREDRAW | CS_VREDRAW;
	mainwndclass.lpfnWndProc = WndProc;
	mainwndclass.cbClsExtra = 0;
	mainwndclass.cbWndExtra = sizeof(long);
	mainwndclass.hInstance = hInstance;
	mainwndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	mainwndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mainwndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mainwndclass.lpszMenuName = NULL;
	mainwndclass.lpszClassName = "TEST";

	if (!RegisterClass(&mainwndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			"TEST", MB_ICONERROR);
		return 0;
	}

	hwnd1 = CreateWindow("TEST",
		TEXT("Hello World for Windows"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd1, nCmdShow);
	UpdateWindow(hwnd1);


	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;


	WriteFile(handle_out, "This is a test sentence.", 24, NULL, NULL);

	// use the console just like a normal one - printf(), getchar(), ...
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
*/