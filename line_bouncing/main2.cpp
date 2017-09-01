#include <windows.h>

#include "LineBouncer.h"

#include <time.h>

#include "custom_window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	srand((unsigned int)time(NULL));

	MSG Msg;

	
	LineBouncer* main_window = new LineBouncer(hInstance, nullptr);

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