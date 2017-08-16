#include <windows.h>
#include <CommCtrl.h>

//#include "custom_spinbox_tools.h"		
#include "custom_window.h"
#include "custom_spinbox.h"
#include "custom_frame.h"

DYNCSTR g_szClassName = TEXT("myWindowClass");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG Msg;

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
	wops.lpClassName = g_szClassName;
	wops.lpWindowName = TEXT("The title of my window");
	wops.dwStyle = WS_OVERLAPPEDWINDOW;
	wops.x = 10;
	wops.y = 2;
	wops.nWidth = 390;
	wops.nHeight = 400;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	PCSWindow main_window = new CSWindow(cswc, wops, nullptr);
	main_window->set_gridding_space(2, 2);
	main_window->weight_grid_row(0, 4);
	main_window->weight_grid_col(0, 1);

	CSFrame* frame;
	frame = new CSFrame(hInstance, main_window);
	frame->grid(0, 1, 10, 10, 1, 1);
	frame->set_gridding_space(2, 1);

	CSSpinbox* spinbox;
	spinbox = new CSSpinbox(hInstance, frame, 0, 10);
	spinbox->grid(0, 0, 10, 10, 1, 1);

	CSSpinbox* spinbox2;
	//spinbox2 = new CSSpinbox(hInstance, frame, 0, 20);
	//spinbox2->grid(1, 0, 20, 30, 1, 1);

	CSWindow* window;
	wops.lpClassName = "EDIT";
	wops.dwStyle = WS_VISIBLE;
	wops.x = 80;
	wops.y = 100;
	wops.nWidth = 100;
	wops.nHeight = 100;
	window = new CSWindow(wops, main_window);
	window->grid(1, 0, 20, 0, 1, 2);

	main_window->show(nCmdShow);
	main_window->update();


	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	delete main_window;
	main_window = nullptr;
	frame = nullptr;
	spinbox = nullptr;
	spinbox2 = nullptr;
	window = nullptr;

	//return 0;
	return Msg.wParam;
}

