#include <windows.h>
#include <CommCtrl.h>
	
#include "custom_window.h"
#include "custom_spinbox.h"
#include "custom_frame.h"
#include "core_packer_algorithm.h"
#include "custom_button.h"

#include "ColorSB.h"
#include "ColorRMR.h"
#include "ColorDC.h"
#include "ColorDraw.h"

#include <time.h>

DYNCSTR g_szClassName = TEXT("myWindowClass");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	srand((unsigned int)time(NULL));

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
	wops.lpWindowName = TEXT("Random Color Picker");
	wops.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	wops.x = 10;
	wops.y = 2;
	wops.nWidth = 390;
	wops.nHeight = 400;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;

	//PColorMain main_window = new ColorMain(cswc, wops, nullptr);
	//main_window->set_gridding_space(1, 2);

	PCSWindow main_window = new CSWindow(cswc, wops, nullptr);

	//
	//Color Randomizer button and spinboxes
	PColorDC cdc = new ColorDC(hInstance, main_window, 550, 300);
	cdc->set_circle_padding(20);
	cdc->pack(TOP, false, 0, 0);

	CSFrame* f = new CSFrame(hInstance, main_window);
	f->set_gridding_space(2, 4);
	f->pack(TOP, true, 0, 0);

	PColorRMR crmr = new ColorRMR(hInstance, f, "Randomize Circle");
	crmr->grid(0, 3, 20, 20, 1, 1);

	PColorSB red_sb = new ColorSB(hInstance, f, 100);
	red_sb->grid(0, 0, 20, 20, 1, 1);

	PColorSB green_sb = new ColorSB(hInstance, f, 100);
	green_sb->grid(0, 2, 20, 20, 1, 1);

	PColorSB blue_sb = new ColorSB(hInstance, f, 100);
	blue_sb->grid(0, 1, 20, 20, 1, 1);

	crmr->connect_sbs(red_sb, green_sb, blue_sb);
	cdc->connect_circle_sbs(red_sb, green_sb, blue_sb);

	//
	//Background Randomizer and spinboxes

	PColorRMR brmr = new ColorRMR(hInstance, f, "Randomize Background");
	brmr->grid(1, 3, 20, 20, 1, 1);

	red_sb = new ColorSB(hInstance, f, 200);
	red_sb->grid(1, 0, 20, 20, 1, 1);

	green_sb = new ColorSB(hInstance, f, 200);
	green_sb->grid(1, 2, 20, 20, 1, 1);

	blue_sb = new ColorSB(hInstance, f, 200);
	blue_sb->grid(1, 1, 20, 20, 1, 1);

	brmr->connect_sbs(red_sb, green_sb, blue_sb);
	cdc->connect_bk_sbs(red_sb, green_sb, blue_sb);

	PColorDraw cdw = new ColorDraw(hInstance, main_window, "Update Drawing");
	cdw->pack(TOP, false, 20, 20);
	cdw->connect_cdc(cdc);


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

	//return 0;
	return (int)Msg.wParam;
}