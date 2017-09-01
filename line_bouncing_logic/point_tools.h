#pragma once
#include <string>

//
//this section of code is purely for the POINT
//struct

namespace point_tools {

	typedef struct POINT_Template {
		float x;
		float y;
	} POINT;

	std::string get_point_str(POINT point);

	POINT translate_point(POINT point, float trans_x, float trans_y);

	POINT transform_coords(POINT point, POINT current_bleft, POINT current_tright,
		POINT new_bleft, POINT new_blight);

	float get_interpolation_decimal(float start, float end, float value);

	float get_interpolated_value(float start, float end, float interpolation_decimal);
}

bool operator==(point_tools::POINT point1, point_tools::POINT point2);

bool operator!=(point_tools::POINT point1, point_tools::POINT point2);

point_tools::POINT operator+(point_tools::POINT point1, point_tools::POINT point2);