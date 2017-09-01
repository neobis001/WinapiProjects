#pragma once
#include "general_types.h"
#include "BounceMatrix.h"
#include "LocVector.h"

const point_tools::POINT fail_point = { -1.0f , -1.0f };

class BounceProcessor {
private:
	BounceMatrix bounce_matrix;
	LocVector input_vec;
	LocVector target_line_vec;

public:
	BounceProcessor();

	void set_input_vec(LocVector input_vec);

	void set_target_vec(LocVector target_line_vec);

	//solve results:
	//attempt to get unique intersection of two lines from input_vec and target_line_vec info
	//2 ways of failure:
	//  both lines are vertical
	//  the rref doesn't reflect a unique solution (either lines are same or are parallel)
	//2 ways of success return:
	//  if the input_vec is vertical or the target_line_vec is vertical, consider y-intercepts
	//  run an rref on the two lines
	//
	//big note: process_solve does a permanent edit to stored matrix 
	point_tools::POINT process_solve();

	void print_out_bounce_matrix();
};