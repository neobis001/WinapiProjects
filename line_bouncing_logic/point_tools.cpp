#include "point_tools.h"

bool operator==(point_tools::POINT point1, point_tools::POINT point2) {
	if (point1.x == point2.x &&
		point1.y == point2.y) {
		return true;
	}
	else {
		return false;
	}
}

bool operator!=(point_tools::POINT point1, point_tools::POINT point2) {
	return !(point1 == point2);
}

point_tools::POINT operator+(point_tools::POINT point1, point_tools::POINT point2) {
	point_tools::POINT res_point;
	res_point.x = point1.x + point2.x;
	res_point.y = point1.y + point2.y;
	return res_point;
}

std::string point_tools::get_point_str(POINT point)
{
	std::string res_string;
	res_string.append("current x: ");
	res_string.append(std::to_string(point.x));
	res_string.append(" current y: ");
	res_string.append(std::to_string(point.y));

	return res_string;
}

point_tools::POINT point_tools::translate_point(POINT point, float trans_x, float trans_y)
{
	POINT res_point = point;
	res_point.x += trans_x;
	res_point.y += trans_y;

	return res_point;
}

float point_tools::get_interpolation_decimal(float start, float end, float value) {
	float overall_range = end - start;
	float rel_value = value - start;
	return rel_value / overall_range;
}

float point_tools::get_interpolated_value(float start, float end, float interpolation_decimal) {
	return ((end - start) * interpolation_decimal) + start;
}

point_tools::POINT point_tools::transform_coords(POINT point, POINT current_bleft, POINT current_tright,
	POINT new_bleft, POINT new_tright) {
	float current_x_decimal = get_interpolation_decimal(current_bleft.x, current_tright.x, point.x);
	float new_x_coord = get_interpolated_value(new_bleft.x, new_tright.x, current_x_decimal);

	float current_y_decimal = get_interpolation_decimal(current_bleft.y, current_tright.y, point.y);
	float new_y_coord = get_interpolated_value(new_bleft.y, new_tright.y, current_y_decimal);

	return{ new_x_coord, new_y_coord};
}

