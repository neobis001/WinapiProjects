#pragma once
#include "general_types.h"

#define LV_BLEFT_X 0.0f
#define LV_BLEFT_Y 0.0f
#define LV_TRIGHT_X 1.0f
#define LV_TRIGHT_Y 1.0f

class LocVector {
protected:
	float x;
	float y;
	float dirx;
	float diry;
public:
	LocVector();

	LocVector(float x, float y, float dirx, float diry);

	LINECOEFF get_line_coeff();

	bool has_undefined_slope();

	float get_slope();

	//given only some point (x1,y1) and ability to get slope
	//can get y_intercept of line by plugging into point-slope formula
	//y - y1 = m(x - x1), and by rearranging and setting x = 0,
	//will get y = y1 - m*x1
	//"y1 - m*x1" is what this function returns as the y-intercept
	float get_y_intercept();

	float get_x();

	float get_y();

	void report_values();

	bool is_flat_line();

	//by choice, a value of 0.0f implies a positive flatness
	FLATNESS get_flatness();

	//will return QNONE for flat lines (those that lie on x or y axis)
	//only considers direction defined w/ dirx and diry
	//x and y can be in quadrants too, but not considering those
	QUADRANT get_dir_quadrant();

	point_tools::POINT get_tail();

	point_tools::POINT get_tip();

	//given the coeffs in the line equation Ax + By = C
	//can rearrange to get y = (-A/B)*x + C/B
	//then can plug in an x to get a y
	float plug_in_x_for_y(float input_x);

	//given a BOUNCEPOINT, with a defined point and normal
	//create a 2D vector given this instance's dirx and y, reflect it given the normal,
	//then update this instance so that the tail is now the intersection point
	//and the direction values are those represented by the reflected 2D vector
	virtual void reflect_locvector(BOUNCEPOINT bp);

	friend bool operator==(LocVector v1, LocVector v2);
};