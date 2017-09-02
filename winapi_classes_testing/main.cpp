#include <windows.h>
#include <CommCtrl.h>
	
#include "custom_window.h"
#include "custom_spinbox.h"
#include "custom_frame.h"
#include "core_packer_algorithm.h"
#include "custom_updown.h"
#include "custom_edit.h"
#include "main_window_pointer.h"

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
	wops.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	wops.x = 10;
	wops.y = 2;
	wops.nWidth = 390;
	wops.nHeight = 400;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	/*
	PCSWindow main_window = new CSWindow(cswc, wops, nullptr);
	main_window->set_gridding_space(2, 2);
	
	wops.lpClassName = "EDIT";
	wops.dwStyle = WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL;
	wops.x = 80;
	wops.y = 100;
	
	wops.nWidth = 100;
	wops.nHeight = 100;

	CSWindow* a = new CSWindow(wops, main_window);
	a->grid(0, 0, 20, 20, 1, 1);

	CSWindow* b = new CSWindow(wops, main_window);
	b->grid(1, 0, 20, 20, 1, 1);

	CSWindow* c = new CSWindow(wops, main_window);
	c->grid(0, 1, 20, 20, 1, 1);
	*/

	
	MainWinP<CSWindow> main_window = new CSWindow(cswc, wops, nullptr);

	wops.lpClassName = "EDIT";
	wops.dwStyle = WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL;
	wops.x = 80;
	wops.y = 100;

	wops.nWidth = 100;
	wops.nHeight = 100;

	CSWindow* a = new CSWindow(wops, main_window);
	a->pack(TOP, false, 20, 20);

	CSWindow* b = new CSWindow(wops, main_window);
	b->pack(TOP, true, 20, 20);

	CSWindow* c = new CSWindow(wops, main_window);
	c->pack(TOP, false, 20, 20);
	

	main_window->show(nCmdShow);
	main_window->update();

	//s->set_window_pos(NULL, 0, 0, 100, 200, 0);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	//return 0;
	return Msg.wParam;
}

