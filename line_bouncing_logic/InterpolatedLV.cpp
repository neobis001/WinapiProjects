#include "InterpolatedLV.h"

void InterpolatedLV::initialize_current_inter() {
	current_inter_x = inter_tail_x;
	current_inter_y = inter_tail_y;
}

void InterpolatedLV::clamp_value(float starting_range, float ending_range, float& value_input) {
	if (value_input < starting_range) {
		value_input = starting_range;
	}
	else if (value_input > ending_range) {
		value_input = ending_range;
	}
}

bool InterpolatedLV::in_inclusive_range(float starting_range, float ending_range, float current_value) {
	if (current_value >= starting_range && current_value <= ending_range) {
		return true;
	}
	else {
		return false;
	}
}

void InterpolatedLV::check_valid_range(float starting_range, float ending_range, float& current_value) {
	if (ending_range <= starting_range) {
		cerr << "Error (in InterpolatedLV::check_valid_range): starting/ending range create a 0 or negative range"
			<< endl;
		abort();
	}
	else {
		clamp_value(starting_range, ending_range, current_value);
	}
}

float InterpolatedLV::get_interpolated_value(float start, float end, float interpolation_decimal) {
	if (!in_inclusive_range(0.0f, 1.0f, interpolation_decimal)) {
		cerr << "Error (in InterpolatedLV::get_interpolated_value): interpolation_decimal not within [0.0f, 1.0f]" << endl;
		abort();
	}
	return point_tools::get_interpolated_value(start, end, interpolation_decimal);
}

InterpolatedLV::InterpolatedLV() {

}

InterpolatedLV::InterpolatedLV(float x, float y, float dirx, float diry) : LocVector(x, y, dirx, diry) {
	inter_tail_x = this->x;
	inter_tail_y = this->y;
	initialize_current_inter();
}

void InterpolatedLV::set_interpolate_target(point_tools::POINT target) {
	inter_target_x = target.x;
	inter_target_y = target.y;
}

void InterpolatedLV::interpolate_tail(float starting_range, float ending_range, float current_value) {
	check_valid_range(starting_range, ending_range, current_value);

	float interp_decimal = point_tools::get_interpolation_decimal(starting_range, ending_range, current_value);
	current_inter_x = get_interpolated_value(inter_tail_x, inter_target_x, interp_decimal);
	current_inter_y = get_interpolated_value(inter_tail_y, inter_target_y, interp_decimal);
}

void InterpolatedLV::report_current_inter_point() {
	cout << "current_inter_x: " << current_inter_x << " current_inter_y: "
		<< current_inter_y << endl;
}

point_tools::POINT InterpolatedLV::get_current_inter_point() {
	point_tools::POINT inter_point;
	inter_point.x = current_inter_x;
	inter_point.y = current_inter_y;
	return inter_point;
}

void InterpolatedLV::reflect_locvector(BOUNCEPOINT bp) {
	LocVector::reflect_locvector(bp);

	inter_tail_x = still_x; //on reflect, update inter_tail to where last still was
	inter_tail_y = still_y;

	still_x = x;
	still_y = y;
}

point_tools::POINT InterpolatedLV::get_inter_tail_point() {
	point_tools::POINT res_point;
	res_point.x = inter_tail_x;
	res_point.y = inter_tail_y;
	return res_point;
}

point_tools::POINT InterpolatedLV::get_inter_tip_point() {
	point_tools::POINT res_point;
	res_point.x = inter_target_x;
	res_point.y = inter_target_y;
	return res_point;
}

point_tools::POINT InterpolatedLV::get_still_point() {
	point_tools::POINT res_point;
	res_point.x = still_x;
	res_point.y = still_y;
	return res_point;
}

void InterpolatedLV::set_still_point(float x, float y) {
	still_x = x;
	still_y = y;
}