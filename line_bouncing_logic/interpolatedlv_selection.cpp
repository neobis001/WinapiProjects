#include "interpolatedlv_selection.h"

bool inside_exclusive(float min_val, float max_val, float input_val) {
	if (input_val > min_val && input_val < max_val) {
		return true;
	}
	else {
		return false;
	}
}

float select_exclusive_value(float min_range, float max_range,
	const char * value_description) {
	bool success = false;
	float res_value;

	while (!success) {
		cout << "input a value for description (" << value_description << "): ";
		cin >> res_value;

		if (inside_exclusive(min_range, max_range, res_value)) {
			success = true;
		}
		else {
			cout << "Value not in range. Try again." << endl;
		}
	}

	return res_value;
}

point_tools::POINT select_exclusive_point(float window_min_x, float window_max_x,
	float window_min_y, float window_max_y) {
	bool success = false;
	point_tools::POINT res;

	res.x = select_exclusive_value(window_min_x, window_max_x, "X coordinate");
	res.y = select_exclusive_value(window_min_y, window_max_y, "Y coordinate");

	return res;
};

point_tools::POINT select_window_point(const char * point_description) {
	cout << "\n\nBeginning point selection for (" << point_description << ")" << endl;
	return select_exclusive_point(window_min_x, window_max_x, window_min_y,
		window_max_y);
}

point_tools::POINT select_different_window_point(const char * point_description, point_tools::POINT other_point) {
	point_tools::POINT res_point; //don't need to initialize, do-while will ensure one
					 //known vector_tip to do an initial comparison
	do {
		res_point = select_window_point(point_description);

		if (res_point == other_point) {
			cout << "Invalid Point: inputted point and other point must be different" << endl;
		}
	} while (res_point == other_point);
	//while selections are made that make tail and tip equal
	//keep the loop running

	return res_point;
}

InterpolatedLV select_contained_interplv() {
	point_tools::POINT vector_tail = select_window_point("Vector Tail");
	point_tools::POINT vector_tip = select_different_window_point("Vector Tip", vector_tail);

	float dirx = vector_tip.x - vector_tail.x;
	float diry = vector_tip.y - vector_tail.y;
	InterpolatedLV res_vector(vector_tail.x, vector_tail.y, dirx, diry);
	return res_vector;
}
