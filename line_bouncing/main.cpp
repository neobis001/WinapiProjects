#include <windows.h>

#include "LineBouncer.h"

#include <time.h>

#include "custom_window.h"

class Test : public CSWindow {
public:
	Test(HINSTANCE hInstance) {
		CSWNDCLASSEX cswc;
		cswc.style = 0;
		cswc.cbClsExtra = 0;
		cswc.cbWndExtra = 0;
		cswc.hInstance = hInstance;
		cswc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		cswc.hCursor = LoadCursor(NULL, IDC_ARROW);
		cswc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		cswc.lpszMenuName = NULL;
		cswc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		
		CSWNDEXOPS wops;
		wops.dwExStyle = WS_EX_CLIENTEDGE;
		wops.lpClassName = "Test";
		wops.lpWindowName = TEXT("The title of my window");
		wops.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		wops.x = 10;
		wops.y = 2;
		wops.nWidth = 390;
		wops.nHeight = 400;
		wops.hMenu = NULL;
		wops.hInstance = hInstance;
		wops.lpParam = NULL;

		init(cswc, wops, nullptr);
	}

	virtual RETURNMSG ON_WM_CREATE(LPCREATESTRUCT lp_cs) {
		SetTimer(hwnd, 1094, 1000, NULL);
		msg_last_error();
		return create_default_option(0);
	}

	virtual RETURNMSG ON_WM_TIMER(UINT_PTR nIDEvent) {
		msg_here();
		return create_default_option(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	srand((unsigned int)time(NULL));

	MSG Msg;

	/*
	LineBouncer* main_window = new LineBouncer(hInstance, nullptr);

	main_window->show(nCmdShow);
	main_window->update();
	*/

	Test* main_window = new Test(hInstance);
	main_window->show(nCmdShow);
	main_window->update();

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);

	}

	delete main_window;
	main_window = nullptr;

	//return 0;
	return (int)Msg.wParam;
}