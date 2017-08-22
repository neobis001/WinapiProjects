#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include <gl/GL.h>
//#include <gl/glext.h>

//#define GLEW_STATIC
//#include <glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>


const char g_szClassName[] = "myWindowClass";

using namespace std;

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
GLint b = 5;
void display() {
	/* rotate a triangle around */
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	//int a = 0;
	//glGetIntegerv(GL_MAJOR_VERSION, &a);

	glLineWidth((GLfloat)b);

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0, (GLfloat)sin((double)holder));
	glColor3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, (GLfloat) sin((double) holder), 0.0f);
	//glVertex2i(-1, -1);
	glVertex2f(-.9f, -.9f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(.9f, -.9f);
	//glVertex2i(1, -1);
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
	case WM_KEYDOWN:
		switch (wParam) {
		case 0x1B:			/* ESC key */
			PostQuitMessage(0);
			break;
		case 0x4f: //O key
			b--;
			break;
		case 0x50: //P key
			b++;
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

	AllocConsole();
	FILE *fpstdin = stdin, *fpstdout = stdout, *fpstderr = stderr;
	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);

	glm::vec4 vec(1.0f,1.0f,1.0f,1.0f);
	glm::vec4 vec2(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 test;
	glm::vec3 translateVec(1.0f, 2.0f, 3.0f);
	test = glm::translate(test, translateVec);

	glm::mat4::col_type ctype = glm::column(test, 3);
	glm::mat4::col_type c2;
	c2[0] = 29.0f;
	c2[1] = 123.0f;
	c2[2] = 1.2f;
	c2[3] = 245.0f;
	test = glm::column(test, 2, c2);

	for (int row_index = 0; row_index < 4; row_index++) {
		for (int col_index = 0; col_index < 4; col_index++) {
			cout << test[col_index][row_index] << " ";
		}
		cout << endl;
	}

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);

	fclose(fpstdin);
	fclose(fpstdout);
	fclose(fpstderr);

	if (FreeConsole() == 0) {
		MessageBox(NULL, "FreeConsole failed", "", MB_OK);
	}

	return Msg.wParam;
}
