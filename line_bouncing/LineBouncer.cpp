#include "LineBouncer.h"

bool LineBouncer::class_registered = false;

const point_tools::POINT gl_bleft = { GL_BLEFT_X, GL_BLEFY_Y };
const point_tools::POINT gl_tright = { GL_TRIGHT_X, GL_TRIGHT_Y };
const point_tools::POINT lv_bleft = { LV_BLEFT_X, LV_BLEFT_Y };
const point_tools::POINT lv_tright = { LV_TRIGHT_X, LV_TRIGHT_Y };

void LineBouncer::do_line_settings(GLubyte r, GLubyte g, GLubyte b, float width) {
	glColor3ub(r, g, b);
	glLineWidth(width);
}

void LineBouncer::render_line(point_tools::POINT& tail, point_tools::POINT& tip) {
	glBegin(GL_LINE_STRIP);
	glVertex2f(tail.x, tail.y);
	glVertex2f(tip.x, tip.y);
	glEnd();
}

void LineBouncer::transform_line_ends(point_tools::POINT& tail, point_tools::POINT& tip) {
	tail = point_tools::transform_coords(tail, lv_bleft, lv_tright, gl_bleft, gl_tright);
	tip = point_tools::transform_coords(tip, lv_bleft, lv_tright, gl_bleft, gl_tright);
}

float LineBouncer::msec_to_sec(float msec) {
	return msec / 1000;
}

void LineBouncer::setup_bounce_timer(HWND hwnd_in_creation) {
	UINT delay_send = sec_to_msec(frame_delay_sec);
	UINT_PTR timer_custom_id = SetTimer(hwnd_in_creation, BOUNCER_TIMER_ID, delay_send, NULL);

	if (timer_custom_id == 0) {
		msg_force_quit("Error (in LineBouncer::ON_WM_CREATE): SetTimer failed to make a timer");
		abort();
	}
}

void LineBouncer::update_line_target() {
	target_bp = find_target_bpoint(main_line);
	main_line.set_interpolate_target(target_bp.p);
}

void LineBouncer::check_counter_end() {
	if (counter >= sec_per_bounce) {
		counter = 0.0f;

		main_line.reflect_locvector(target_bp);
		update_line_target(); 
		main_line.interpolate_tail(0.0f, sec_per_bounce, 0.0f); //after the reflection
		//and the finding of the new target BOUNCEPOINT, the current_inter_x and current_inter_y
		//are interpolated to the 1.0 position from the most previous update_interpolation
		//and on the reflect_locvector, assigning still_x = x and still_y = y (aka assinging it to the tail)
		//puts it on the same place on the current_inter point, causing a no-line draw on the next
		//WM_PAINT
		//so call an interpolate_tail at 0 to update the current_inter to where it should be before the 
		//next WM_PAINT message
	}
}

void LineBouncer::update_interpolation() {
	counter += frame_delay_sec;
	main_line.interpolate_tail(0.0f, sec_per_bounce, counter);
}

void LineBouncer::check_frame_settings() {
	if (sec_per_bounce < frame_delay_sec) {
		msg_force_quit("Error (in LineBouncer::check_frame_settings): seconds per bounce is "\
			"less than frame delay seconds, not allowed, see member var. comments");
	}
}

void LineBouncer::process_counter() {
	update_interpolation();
	check_counter_end();
}

UINT LineBouncer::sec_to_msec(float sec) {
	UINT conversion = (UINT)(sec * 1000);
	return conversion;
}

void LineBouncer::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	do_line_settings(255, 0, 0, 5.0f);
	point_tools::POINT tail = main_line.get_current_inter_point();
	point_tools::POINT tip = main_line.get_still_point();
	transform_line_ends(tail, tip);
	render_line(tail, tip);

	glFlush();
}

void LineBouncer::cleanup_opengl_context() {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, rc_hdc);
}

void LineBouncer::create_opengl_context(HDC hdc) {
	hglrc = wglCreateContext(hdc);
	if (hglrc == NULL) {
		msg_force_quit("Error (in LineBouncer::connect_opengl_context):"\
			" wglCreateContext failed to make hglrc");
	}
}

//must release dc on window destroy
void LineBouncer::connect_opengl_context() {
	HDC rc_hdc = GetDC(hwnd);
	
	create_opengl_context(rc_hdc);

	BOOL current_res = wglMakeCurrent(rc_hdc, hglrc);
	if (current_res == FALSE) {
		msg_force_quit("Error (in LineBouncer::connect_opengl_context): attempt to make hglrc"\
			" current opengl context failed");
	}
}

void LineBouncer::apply_pixel_format() {
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA;
	pfd.iPixelType = PFD_TYPE_RGBA; //PFD_TYPE_RGBA is also 0
	pfd.cColorBits = 32;

	int pixel_format = ChoosePixelFormat(hdc, &pfd);
	if (pixel_format == 0) {
		msg_force_quit("Error (in LineBouncer::apply_pixel_format): ChoosePixelFormat failed");
	}

	BOOL set_res = SetPixelFormat(hdc, pixel_format, &pfd);
	if (set_res == FALSE) {
		msg_force_quit("Error (in LineBouncer::apply_pixel_format): SetPixelFormat failed");
	}

	ReleaseDC(hwnd, hdc);
}


void LineBouncer::prepare_class_and_size(CSWNDEXOPS& wops) {
	wops.dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; //WS_CHILD considered
	  //but with no parent, won't be added

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = 300;
	rect.bottom = 300;
	AdjustWindowRectEx(&rect, wops.dwStyle, FALSE, wops.dwExStyle); //doing Ex version
	  //just in case I decide to add a style there

	wops.nWidth = rect.right - rect.left;
	wops.nHeight = rect.bottom - rect.top;
}

void LineBouncer::init_with_defaults(HINSTANCE hInstance, PCSWindow parent) {
	CSWNDEXOPS wops;
	wops.dwExStyle = 0;
	wops.lpClassName = frame_class;
	wops.lpWindowName = TEXT("LineBouncer Title");
	wops.x = CW_USEDEFAULT;
	wops.y = CW_USEDEFAULT;
	wops.hMenu = NULL;
	wops.hInstance = hInstance;
	wops.lpParam = NULL;
	prepare_class_and_size(wops);

	if (!class_registered) {
		CSWNDCLASSEX cswndclass;
		cswndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		cswndclass.cbClsExtra = 0;
		cswndclass.cbWndExtra = 0;
		cswndclass.hInstance = hInstance;
		cswndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		cswndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		cswndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		cswndclass.lpszMenuName = NULL;
		cswndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		init(cswndclass, wops, parent);
		class_registered = true;
	}
	else {
		init(wops, parent);
	}
}

LineBouncer::LineBouncer() {

}

LineBouncer::LineBouncer(HINSTANCE hInstance, PCSWindow parent): main_line(0.4f, 0.4f, 0.45f, -0.1f) {
	check_frame_settings();

	init_with_defaults(hInstance, parent);
	apply_pixel_format();
	connect_opengl_context();
}

RETURNMSG LineBouncer::ON_WM_PAINT() {
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	render();

	EndPaint(hwnd, &ps);

	return create_default_option(0);
}

RETURNMSG LineBouncer::ON_WM_CLOSE() {
	cleanup_opengl_context();
	DestroyWindow(hwnd); //looks like docs say default behavior is to destroy window
	  //if processing WM_CLOSE manually and returning 0, should destroy window manually
	return create_default_option(0);
}

RETURNMSG LineBouncer::ON_WM_SIZE(UINT resize_type, INT new_width, INT new_height) {
	glViewport(0, 0, new_width, new_height);
	InvalidateRect(hwnd, NULL, TRUE);
	return create_default_option(0);
}


RETURNMSG LineBouncer::ON_WM_ERASEBKGND(HDC hdc) {
	return create_default_option(1); //repainting is 2-step process
	  //with WM_ERASEBKGND and then WM_PAINT
	  //to avoid flickering (and since WM_PAINT covers entire client area)
	  //return 1 to bypass ERASEBKGND painting
}


RETURNMSG LineBouncer::ON_WM_CREATE(LPCREATESTRUCT lp_cs, HWND hwnd_in_creation) {
	update_line_target();
	point_tools::POINT initial_still = main_line.get_tip();
	main_line.set_still_point(initial_still.x, initial_still.y);

	setup_bounce_timer(hwnd_in_creation);
	return create_default_option(0);
}

RETURNMSG LineBouncer::ON_WM_TIMER(UINT_PTR nIDEvent) {
	if (nIDEvent == BOUNCER_TIMER_ID) {
		process_counter();

		InvalidateRect(hwnd, NULL, TRUE);
		return create_defproc_option();
	}
	else {
		return create_defproc_option();
	}
}