#include "target_line_finding.h"

LocVector left_w_edge(BOTTOM_LEFT_X, BOTTOM_LEFT_Y, 0.0f, 1.0f);
LocVector bottom_w_edge(BOTTOM_LEFT_X, BOTTOM_LEFT_Y, 1.0f, 0.0f);
LocVector right_w_edge(BOTTOM_RIGHT_X, BOTTOM_RIGHT_Y, 0.0f, 1.0f);
LocVector top_w_edge(TOP_LEFT_X, TOP_LEFT_Y, 1.0f, 0.0f);

//pretty sure normal vectors should be normalized, and with top,left,right,bottom
//cases, w/ 0 and 1 values, don't need to call a normalization funciton
glm::vec2 down_vec(0.0f, -1.0f);
glm::vec2 up_vec(0.0f, 1.0f);
glm::vec2 right_vec(1.0f, 0.0f);
glm::vec2 left_vec(-1.0f, 0.0f);

LocVector find_target_line_from_flatness(FLATNESS flatness) {
	switch (flatness) {
	case FLATNESS::POS_X: {
		return right_w_edge;
	}
	case FLATNESS::NEG_X: {
		return left_w_edge;
	}
	case FLATNESS::POS_Y: {
		return top_w_edge;
	}
	case FLATNESS::NEG_Y: {
		return bottom_w_edge;
	}
	default: { //must be NONE, default should be okay then 
		cerr << "Error (in LineProcessor::find_target_lines): line flatness is NONE," << endl <<
			"shouldn't be at this stage" << endl;
		abort();
	}
	}
}

glm::vec2 get_window_bounce_vec(LocVector target_line) {
	if (target_line == left_w_edge) {
		glm::vec2 res(1.0f, 0.0f);
		return res;
	}
	else if (target_line == top_w_edge) {
		glm::vec2 res(0.0f, -1.0f);
		return res;
	}
	else if (target_line == right_w_edge) {
		glm::vec2 res(-1.0f, 0.0f);
		return res;
	}
	else { //assumed bottom edge case
		glm::vec2 res(0.0f, 1.0f);
		return res;
	}
}

float get_point_distance(point_tools::POINT p1, point_tools::POINT p2) {
	float squared_part = pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
	return pow(squared_part, 0.5f);
}

BOUNCEPOINT find_closer_bpoint(LocVector input_line, LocVector target_line1, LocVector target_line2) {
	BOUNCEPOINT res_bp;

	BounceProcessor bp1;
	bp1.set_input_vec(input_line);
	bp1.set_target_vec(target_line1);
	point_tools::POINT point1 = bp1.process_solve();

	BounceProcessor bp2;
	bp2.set_input_vec(input_line);
	bp2.set_target_vec(target_line2);
	point_tools::POINT point2 = bp2.process_solve();

	point_tools::POINT input_tail = input_line.get_tail();
	float distance1 = get_point_distance(input_tail, point1);
	float distance2 = get_point_distance(input_tail, point2);

	//NOTE: the time when they can be equal is when the input vec hits a corner
	//like points towards top right corner of window
	if (distance1 <= distance2) {
		res_bp.bounce_vec = get_window_bounce_vec(target_line1);
		res_bp.p = point1;
	}
	else {
		res_bp.bounce_vec = get_window_bounce_vec(target_line2);
		res_bp.p = point2;
	}

	return res_bp;
}

BOUNCEPOINT find_target_bpoint(LocVector input_line) {
	if (input_line.is_flat_line()) {
		LocVector target_line = find_target_line_from_flatness(input_line.get_flatness());
		BounceProcessor bp;
		bp.set_input_vec(input_line);
		bp.set_target_vec(target_line);

		BOUNCEPOINT res_bp;
		res_bp.bounce_vec = get_window_bounce_vec(target_line);
		res_bp.p = bp.process_solve();
		return res_bp;
	}
	else {
		QUADRANT dir_quad = input_line.get_dir_quadrant();

		if (dir_quad == QUADRANT::Q1) {
			return find_closer_bpoint(input_line, top_w_edge, right_w_edge);
		}
		else if (dir_quad == QUADRANT::Q2) {
			return find_closer_bpoint(input_line, top_w_edge, left_w_edge);
		}
		else if (dir_quad == QUADRANT::Q3) {
			return find_closer_bpoint(input_line, left_w_edge, bottom_w_edge);
		}
		else { //assumed quadrant 4
			return find_closer_bpoint(input_line, bottom_w_edge, right_w_edge);
		}
	}
}
