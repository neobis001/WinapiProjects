#pragma once

#include "LocVector.h"
#include "point_tools.h"

class InterpolatedLV : public LocVector {
private:
	float inter_tail_x = 0.0f;
	float inter_tail_y = 0.0f;

	float current_inter_x = 0.0f;
	float current_inter_y = 0.0f;

	float inter_target_x = 0.0f;
	float inter_target_y = 0.0f;

	float still_x = 0.0f;
	float still_y = 0.0f;
	
	void initialize_current_inter();

	//assumes valid range
	void clamp_value(float starting_range, float ending_range, float& value_input);

	//used to not allow values outside of inclusive range, but now that allowing it
	//MAY CONSIDER REMOVING THIS FUNCTION
	bool in_inclusive_range(float starting_range, float ending_range, float current_value);

	//checks if starting and ending range are valid and then clamps the current_value to
	//the range if needed
	//since current_value could go reasonably out of range due to timing code, clamping is best
	//option here
	void check_valid_range(float starting_range, float ending_range, float& current_value);

	float get_interpolated_value(float start, float end, float interpolation_decimal);

public:
	InterpolatedLV();

	InterpolatedLV(float x, float y, float dirx, float diry);

	void set_interpolate_target(point_tools::POINT target);

	void interpolate_tail(float starting_range, float ending_range, float current_value);

	void report_current_inter_point();

	point_tools::POINT get_current_inter_point();

	point_tools::POINT get_inter_tail_point();

	point_tools::POINT get_inter_tip_point();

	point_tools::POINT get_still_point();

	void set_still_point(float x, float y);


	//reflect a locvector like normal
	//then update this instance so that after the reflection
	//given a reflected tail and tip
	//the interpolating tail_x/y values are updates for future 
	//interpolations
	virtual void reflect_locvector(BOUNCEPOINT bp);

};