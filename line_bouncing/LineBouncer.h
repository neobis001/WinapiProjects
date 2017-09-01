#pragma once
#include <Windows.h> //in windows, must include windows.h before gl.h
#include <GL/GL.h>

#include "custom_window.h"
#include "line_bouncing_logic.h"


class LineBouncer;
#define BOUNCER_TIMER_ID 1002

#define GL_BLEFT_X -1.0f
#define GL_BLEFY_Y -1.0f
#define GL_TRIGHT_X 1.0f
#define GL_TRIGHT_Y 1.0f

class LineBouncer : public CSWindow {
private:
	static bool class_registered;
	DYNCSTR frame_class = TEXT("LINEBOUNCER_CLASS");

	HGLRC hglrc;
	HDC rc_hdc;

	BOUNCEPOINT target_bp;
	InterpolatedLV main_line;

	float counter = 0.0f;
	float holder = -0.5f;
	float test_increment_x = 0.05f;

	//usage
	//given a frame_delay_sec of 1.0f and sec_per_bounce of 2.0f
	//2 frames will run
	//after the 2nd frame, bounce finishes counter resets
	//and after 1.0f frame_delay sec, 1st frame of next bounce is run
	float frame_delay_sec = 0.2f;

	//note if sec_per_bounce less than frame_delay_sec, bounce will be clamped to
	//frame_delay_sec since there's no way to time anything less than that
	float sec_per_bounce = 2.0f;

	void do_line_settings(GLubyte r, GLubyte g, GLubyte b, float width);

	void render_line(point_tools::POINT& tail, point_tools::POINT& tip);

	//transform a given tail and tip from their current coordinates settings (defined in 
	//LocVector.h) to opengl coordinates (define in this header file)
	void transform_line_ends(point_tools::POINT& tail, point_tools::POINT& tip);

	float msec_to_sec(float msec);

	//using the frame delay var, create a timer that'll give WM_TIMER messages 
	//after a given frame delay
	void setup_bounce_timer(HWND hwnd_in_creation);

	void update_line_target();

	UINT sec_to_msec(float sec);

	void check_counter_end();

	void update_interpolation();

	//see comments on member vars. for check
	//also w/ incorrect frame settings, would translate very confusingly into
	//InterpolatedLV class
	void check_frame_settings();

	void process_counter();

	void render();

	void prepare_class_and_size(CSWNDEXOPS& cswndclassex);

	void apply_pixel_format();

	void connect_opengl_context();

	void create_opengl_context(HDC hdc);

	void cleanup_opengl_context();

protected:

	void init_with_defaults(HINSTANCE hInstance, PCSWindow parent);

public:
	LineBouncer();

	LineBouncer(HINSTANCE hInstance, PCSWindow parent);

	virtual RETURNMSG ON_WM_PAINT();

	//On closing the window, disconnect the current opengl rendering context
	//and release the rc_hdc that was used while the window was open
	virtual RETURNMSG ON_WM_CLOSE();

	//On resizing the window, stretch the opengl drawing coordinates to fit the
	//entire client area
	virtual RETURNMSG ON_WM_SIZE(UINT resize_type, INT new_width, INT new_height);

	virtual RETURNMSG ON_WM_ERASEBKGND(HDC hdc);

	virtual RETURNMSG ON_WM_CREATE(LPCREATESTRUCT lp_cs, HWND hwnd_in_creation);

	virtual RETURNMSG ON_WM_TIMER(UINT_PTR nIDEvent);

};