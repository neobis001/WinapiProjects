#include <Windows.h>
#include <stdio.h>
#include <math.h>

#include <gl/GL.h>
#include <gl/glext.h>

const char g_szClassName[] = "myWindowClass";

/*
void *GetAnyGLFuncAddress(const char *name)
{
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}

	return p;
}
*/

int holder = 0;
void display() {
	/* rotate a triangle around */
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0, (GLfloat)sin((double)holder));
	glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex2f(0, (GLfloat)sin((double)holder));
	glColor3f(0.0f, (GLfloat) sin((double) holder), 0.0f);
	glVertex2i(-1, -1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);
	glEnd();
	glFlush();
}

PAINTSTRUCT ps;
// Step 4: the Window Procedure
#define ID_TIMER 1001
UINT nTimer;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		nTimer = SetTimer(hwnd, ID_TIMER, 50, NULL);
		return 0;
	case WM_PAINT:
		display();
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_TIMER:
		holder += 50;
		BeginPaint(hwnd, &ps);
		display();
		EndPaint(hwnd, &ps);
		return 0;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hwnd, WM_PAINT, 0, 0);
		return 0;
	case WM_CHAR:
		switch (wParam) {
		case 27:			/* ESC key */
			PostQuitMessage(0);
			break;
		}
		return 0;

	case WM_CLOSE:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		FILE *fpstdin = stdin, *fpstdout = stdout, *fpstderr = stderr;

		freopen_s(&fpstdin, "CONIN$", "r", stdin);
		freopen_s(&fpstdout, "CONOUT$", "w", stdout);
		freopen_s(&fpstderr, "CONOUT$", "w", stderr);

		printf("This is a test of the attached console");
		FreeConsole();
	}


	//Step 1: Registering the Window Class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_OWNDC;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
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
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 200,
		NULL, NULL, hInstance, NULL);

	if(hwnd == (void*)0)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA;
	pfd.iPixelType = 0;
	pfd.cColorBits = 32;

	int pix_format_index = ChoosePixelFormat(hdc, &pfd);
	if (pix_format_index == 0) {
		MessageBox(NULL, "ChoosePixelFormat Error", "", MB_OK);
	}

	BOOL res = SetPixelFormat(hdc, pix_format_index, &pfd);
	if (res == FALSE) {
		MessageBox(NULL, "SetPixelFormat Error", "", MB_OK);
	}


	ReleaseDC(hwnd, hdc);

	hdc = GetDC(hwnd);
	HGLRC hglrc = wglCreateContext(hdc);

	wglMakeCurrent(hdc, hglrc);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	/*
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		MessageBox(NULL, "error", "", MB_OK);
	}*/

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);

	return Msg.wParam;
}
