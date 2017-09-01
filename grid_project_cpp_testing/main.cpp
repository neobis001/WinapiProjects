#include <windows.h>
#include <CommCtrl.h>
	
#include "custom_window.h"
#include "custom_spinbox.h"
#include "custom_frame.h"
#include "core_packer_algorithm.h"
#include "custom_updown.h"
#include "custom_edit.h"

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

	PCSWindow main_window = new CSWindow(cswc, wops, nullptr);
	main_window->set_gridding_space(1, 2);

	//CSSpinbox* s = new CSSpinbox(hInstance, main_window, 0, 10);

	
	wops.lpClassName = "EDIT";
	wops.dwStyle = WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL;
	wops.x = 80;
	wops.y = 100;
	wops.nWidth = 500;
	wops.nHeight = 400;
	//PCSEdit window = new CSEdit(hInstance, main_window);
	//window->pack(TOP, false, 0, 0);
	
	/*
	wops.nWidth = 100;
	wops.nHeight = 200;
	CSFrame* f = new CSFrame(hInstance, main_window);
	f->set_gridding_space(2, 3);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			CSSpinbox* w = new CSSpinbox(hInstance, f);
			CSEdit* e = new CSEdit(hInstance, w);
			CSUpdown* u = new CSUpdown(hInstance, w);
			w->setup_sb_windows(e, u, 0, 10, 5);

			w->grid(i, j, 20, 20, 1, 1);
		}
	}
	f->pack(TOP, true, 0, 0);
	*/

	
	wops.nWidth = 100;
	wops.nHeight = 100;

	/*
	CSWindow* w3 = new CSWindow(wops, main_window);
	w3->pack(LEFT, false, 0, 0);

	CSWindow* w4 = new CSWindow(wops, main_window);
	w4->pack(BOTTOM, false, 0, 0);

	CSWindow* w5 = new CSWindow(wops, main_window);
	w5->pack(LEFT, false, 0, 0);

	CSWindow* w6 = new CSWindow(wops, main_window);
	w6->pack(TOP, false, 0, 0);

	CSWindow* w7 = new CSWindow(wops, main_window);
	w7->pack(LEFT, false, 0, 0);
	*/

	CSWindow* a = new CSWindow(wops, main_window);
	a->pack(TOP, false, 0, 0);

	CSWindow* b = new CSWindow(wops, main_window);
	b->pack(TOP, true, 0, 0);

	CSWindow* c = new CSWindow(wops, main_window);
	c->pack(TOP, false, 0, 0);

	main_window->show(nCmdShow);
	main_window->update();

	//s->set_window_pos(NULL, 0, 0, 100, 200, 0);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	delete main_window;
	main_window = nullptr;

	//return 0;
	return Msg.wParam;
}

