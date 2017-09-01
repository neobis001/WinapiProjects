#pragma once
#include "InterpolatedLV.h"

//
//
//this code section is written for selecting a point within a box w/
//lower left corner at (0,0) and top right corner at (1,0)

const float window_min_x = 0.0f;
const float window_max_x = 1.0f;
const float window_min_y = 0.0f;
const float window_max_y = 1.0f;

bool inside_exclusive(float min_val, float max_val, float input_val);

float select_exclusive_value(float min_range, float max_range,
	const char * value_description);

point_tools::POINT select_exclusive_point(float window_min_x, float window_max_x,
	float window_min_y, float window_max_y);

point_tools::POINT select_window_point(const char * point_description);

point_tools::POINT select_different_window_point(const char * point_description, point_tools::POINT other_point);

InterpolatedLV select_contained_interplv();