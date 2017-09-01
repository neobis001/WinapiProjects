#include "BounceProcessor.h"

BounceProcessor::BounceProcessor() {

}

void BounceProcessor::set_input_vec(LocVector input_vec) {
	this->input_vec = input_vec;
}

void BounceProcessor::set_target_vec(LocVector target_line_vec) {
	this->target_line_vec = target_line_vec;
}

point_tools::POINT BounceProcessor::process_solve() {
	if (input_vec.has_undefined_slope() && target_line_vec.has_undefined_slope()) {
		cout << "Warning (in BounceProcessor::process_solve): both line vectors are vertical, " <<
			endl << "no unique solution possible, returning {-1,-1}" << endl;
		return fail_point;
	}
	else {
		if (input_vec.has_undefined_slope()) {
			//return y intercept of target_line_vec relative to input_vec line
			point_tools::POINT y_int_point;
			y_int_point.x = input_vec.get_x();
			y_int_point.y = target_line_vec.get_y_intercept();
			return y_int_point;
		}
		else if (target_line_vec.has_undefined_slope()) {
			//return y intercept of input_vec line relative to target_line_vec
			point_tools::POINT y_int_point;
			y_int_point.x = target_line_vec.get_x();
			y_int_point.y = input_vec.plug_in_x_for_y(y_int_point.x);
			return y_int_point;
		}
		else {
			bounce_matrix.setup_matrix(input_vec, target_line_vec);
			bounce_matrix.run_rref();
			if (!bounce_matrix.rref_has_unique_solution()) {
				cout << "Warning (in BounceProcessor::process_solve): rref doesn't reflect a unique solution, " <<
					endl << "check code, returning {-1, -1}" << endl;
				return fail_point;
			}
			else {
				return bounce_matrix.get_rref_solution();
			}
		}
	}
}

void BounceProcessor::print_out_bounce_matrix() {
	bounce_matrix.print_matrix();
}