#include <Windows.h>

#include <CommCtrl.h>
#pragma comment(linker,"\"/manifestdependency:type                  = 'win32' \
                                              name                  = 'Microsoft.Windows.Common-Controls' \
                                              version               = '6.0.0.0' \
                                              processorArchitecture = '*' \
                                              publicKeyToken        = '6595b64144ccf1df' \
                                              language              = '*'\"")

const char g_szClassName[] = "myWindowClass";

HWND udwnd;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_NOTIFY:
		LPNMHDR lp_nmhdr = (LPNMHDR)lParam;
		if (lp_nmhdr->code == UDN_DELTAPOS) {
			LPNMUPDOWN lp_nmupdown = (LPNMUPDOWN)lParam;
			CHAR buffer[200];
			wsprintf(buffer, "iPos %i and iDelta %i", lp_nmupdown->iPos, lp_nmupdown->iDelta);
			MessageBox(NULL, buffer, "", MB_OK);
		}

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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"The title of my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 200,
		NULL, NULL, hInstance, NULL);

	if(hwnd == (void*)0)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND edit = CreateWindowEx(0, "EDIT", "text", WS_VISIBLE | WS_CHILD, 0, 0, 100, 100, hwnd, NULL, NULL, 0);

	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_UPDOWN_CLASS;
	BOOL init_res = InitCommonControlsEx(&iccx);
	if (init_res == FALSE) {
		MessageBox(NULL, TEXT("Warning(in initialize_common_controls) : InitCommonControlsEx failed"), "", MB_OK);
	}
	udwnd = CreateWindowEx(0, UPDOWN_CLASS, "Test", WS_VISIBLE | WS_CHILD | UDS_ARROWKEYS |
		UDS_SETBUDDYINT, 
		150, 0, 20, 100, hwnd, (HMENU)1001, hInstance, 0);
	SendMessage(udwnd, UDM_SETRANGE, 0, MAKELPARAM(10, 0));
	SendMessage(udwnd, UDM_SETBUDDY, (WPARAM)edit, 0);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
