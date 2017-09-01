#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <cassert>
#include <cmath>

FILE *stream;

using namespace std;

//meant to store floats from eq. Ax + By = C
typedef struct LINECOEFF_Template {
	float A;
	float B;
	float C;
} LINECOEFF; 


//meant to symbolize 4 quadrants
//if considering the x or y axis, the quadrant would be QNONE
enum QUADRANT {
	Q1,
	Q2,
	Q3,
	Q4,
	QNONE
};

//
//this section of code is purely for the POINT
//struct

typedef struct POINT_Template {
	float x;
	float y;
} POINT;

bool operator==(POINT point1, POINT point2) {
	if (point1.x == point2.x &&
		point1.y == point2.y) {
		return true;
	}
	else {
		return false;
	}
}

bool operator!=(POINT point1, POINT point2) {
	return !(point1 == point2);
}

//
//BOUNCEPOINT section, just a definition

typedef struct BOUNCEPOINT_Template {
	glm::vec2 bounce_vec;
	POINT p;
} BOUNCEPOINT;


//
//
//this section of code is for purely the FLATNESS
//enumeration

enum FLATNESS {
	POS_X,
	NEG_X,
	POS_Y,
	NEG_Y,
	NONE
};


const char * get_flatness_str(FLATNESS flatness) {
	switch (flatness) {
	case POS_X: {
		return "POS_X";
	}
	case NEG_X: {
		return "NEG_X";
	} 
	case POS_Y: {
		return "POS_Y";
	}
	case NEG_Y: {
		return "NEG_Y";
	}
	case NONE: {
		return "NONE";
	}
	default: {
		cout << "Warning (in get_flatness_str): no matching value, returning \"No match\"" << endl;
		return "No match";
	}
	}
}

class InterpolatedLV;

class LocVector {
protected:
	float x;
	float y;
	float dirx;
	float diry;
public:
	LocVector() {

	}

	LocVector(float x, float y, float dirx, float diry) {
		this->x = x;
		this->y = y;

		if (dirx == 0.0f) {
			cerr << "Warning (in LocVector constructor): if dirx is 0, slope will be undefined, keep in mind" << endl;
		}
		this->dirx = dirx;
		this->diry = diry;
	}

	LINECOEFF get_line_coeff() {
		LINECOEFF res;
		if (has_undefined_slope()) {
			cerr << "Warning (in LocatedVector::get_line_coeff): slope is undefined, " <<
				"returning LINECOEFF of -1's" << endl;
			res.A = -1;
			res.B = -1;
			res.C = -1;
		} 
		else {
			float slope = get_slope();
			float y_intercept = get_y_intercept();
			res.A = -slope;
			res.B = 1;
			res.C = y_intercept;
		}
		return res;
	}
	
	bool has_undefined_slope() {
		if (dirx == 0.0f) {
			return true;
		}
		else {
			return false;
		}
	}

	float get_slope() {
		if (has_undefined_slope()) {
			cerr << "Warning (in LocatedVector::get_slope): slope is undefined, returning -1" << endl;
			return -1.0f;
		}
		else {
			return diry / dirx;
		}
	}

	//given only some point (x1,y1) and ability to get slope
	//can get y_intercept of line by plugging into point-slope formula
	//y - y1 = m(x - x1), and by rearranging and setting x = 0,
	//will get y = y1 - m*x1
	//"y1 - m*x1" is what this function returns as the y-intercept
	float get_y_intercept() {
		float slope = get_slope();
		return y - slope*x;
	}

	float get_x() {
		return x;
	}

	float get_y() {
		return y;
	}

	void report_values() {
		cout << "x: " << x << " y: " << y << " dirx: " << dirx << " diry: " << diry << endl;
 	}

	bool is_flat_line() {
		if (has_undefined_slope() || get_slope() == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	//by choice, a value of 0.0f implies a positive flatness
	FLATNESS get_flatness() {
		if (!is_flat_line()) {
			return FLATNESS::NONE;
		}
		else {
			if (has_undefined_slope()) {
				if (diry >= 0.0f) {
					return FLATNESS::POS_Y;
				}
				else {
					return FLATNESS::NEG_Y;
				}
			}
			else {
				if (dirx >= 0.0f) {
					return FLATNESS::POS_X;
				}
				else {
					return FLATNESS::NEG_X;
				}
			}
		}
	}

	//will return QNONE for flat lines (those that lie on x or y axis)
	//only considers direction defined w/ dirx and diry
	//x and y can be in quadrants too, but not considering those
	QUADRANT get_dir_quadrant() {
		if (is_flat_line()) {
			return QUADRANT::QNONE;
		}
		else {
			if (diry > 0) {
				if (dirx > 0) {
					return QUADRANT::Q1;
				}
				else { //if it's not a flat line, must be < 0, not == 0
					return QUADRANT::Q2;
				}
			}
			else { //if it's not a flat line, must be < 0, not == 0
				if (dirx > 0) {
					return QUADRANT::Q4;
				}
				else { 
					return QUADRANT::Q3;
				}
			}
		}
	}

	POINT get_tail() {
		POINT tail;
		tail.x = x;
		tail.y = y;
		return tail;
	}

	POINT get_tip() {
		POINT tip;
		tip.x = x;
		tip.y = y;
		return tip;
	}

	//given the coeffs in the line equation Ax + By = C
	//can rearrange to get y = (-A/B)*x + C/B
	//then can plug in an x to get a y
	float plug_in_x_for_y(float input_x) {
		LINECOEFF coeffs = get_line_coeff();
		float A = coeffs.A;
		float B = coeffs.B;
		float C = coeffs.C;

		return (-A / B)*input_x + (C / B);
	}

	//given a BOUNCEPOINT, with a defined point and normal
	//create a 2D vector given this instance's dirx and y, reflect it given the normal,
	//then update this instance so that the tail is now the intersection point
	//and the direction values are those represented by the reflected 2D vector
	virtual void reflect_locvector(BOUNCEPOINT bp) {
		glm::vec2 current_vec;
		current_vec.x = dirx;
		current_vec.y = diry;

		glm::vec2 reflected_vec = glm::reflect(current_vec, bp.bounce_vec);

		x = bp.p.x;
		y = bp.p.y;
		dirx = reflected_vec.x;
		diry = reflected_vec.y;
	}

	friend bool operator==(LocVector v1, LocVector v2);
};

bool operator==(LocVector v1, LocVector v2) {
	if (v1.x == v2.x &&
		v1.y == v2.y &&
		v1.dirx == v2.dirx &&
		v1.diry == v2.diry) {
		return true;
	}
	else {
		return false;
	}
}

class InterpolatedLV : public LocVector {
private:
	float inter_tail_x = 0.0f;
	float inter_tail_y = 0.0f;

	float current_inter_x = 0.0f;
	float current_inter_y = 0.0f;

	float inter_target_x = 0.0f;
	float inter_target_y = 0.0f;


	//assumes valid range
	bool in_inclusive_range(float starting_range, float ending_range, float current_value) {
		if (current_value >= starting_range && current_value <= ending_range) {
			return true;
		}
		else {
			return false;
		}
	}

	void check_valid_range(float starting_range, float ending_range, float current_value) {
		if (ending_range <= starting_range) {
			cerr << "Error (in InterpolatedLV::check_valid_range): starting/ending range create a 0 or negative range"
				<< endl;
			abort();
		}
		else if (!in_inclusive_range(starting_range, ending_range, current_value)) {
			cerr << "Error (in InterpolatedLV::check_valid_range): current_value not in validated range" << endl;
			abort();
		}
	}

	//assume value is good
	float get_interpolation_decimal(float start, float end, float value) {
		float overall_range = end - start;
		float rel_value = value - start;
		return rel_value / overall_range;
	}

	float get_interpolated_value(float start, float end, float interpolation_decimal) {
		if (!in_inclusive_range(0.0f, 1.0f, interpolation_decimal)) {
			cerr << "Error (in InterpolatedLV::get_interpolated_value): interpolation_decimal not within [0.0f, 1.0f]" << endl;
			abort();
		}
		return (end - start) * interpolation_decimal;
	}

public:
	InterpolatedLV() {

	}

	InterpolatedLV(float x, float y, float dirx, float diry) : LocVector(x, y, dirx, diry) {
		inter_tail_x = this->x;
		inter_tail_y = this->y;
	}

	void set_interpolate_target(POINT target) {
		inter_target_x = target.x;
		inter_target_y = target.y;
	}

	void interpolate_tail(float starting_range, float ending_range, float current_value) {
		check_valid_range(starting_range, ending_range, current_value);

		float interp_decimal = get_interpolation_decimal(starting_range, ending_range, current_value);
		current_inter_x = inter_tail_x + get_interpolated_value(inter_tail_x, inter_target_x, interp_decimal);
		current_inter_y = inter_tail_y + get_interpolated_value(inter_tail_y, inter_target_y, interp_decimal);
	}

	void report_current_inter_point() {
		cout << "current_inter_x: " << current_inter_x << " current_inter_y: " 
			<< current_inter_y << endl;
	}

	//reflect a locvector like normal
	//then update this instance so that after the reflection
	//given a reflected tail and tip
	//the interpolating tail_x/y values are updates for future 
	//interpolations
	virtual void reflect_locvector(BOUNCEPOINT bp) {
		LocVector::reflect_locvector(bp);

		inter_tail_x = x;
		inter_tail_y = y;
	}

};


class BounceMatrix {
private:
	glm::mat3x2 bounce_matrix; //note matrix is initialized by glm in rref 
	const int num_cols = 3;
	const int num_rows = 2;

	bool dim_input_is_valid(int input, int dim_size) {
		//doing this not because of considering public access
		//but for self check
		if (input >= 0 && input < dim_size) {
			return true;
		}
		else {
			return false;
		}
	}


public:
	BounceMatrix() {

	}

	void plug_vec_to_row(LocVector loc_vector, int row_number) {
		if (!dim_input_is_valid(row_number, num_rows)) {
			cerr << "Error (in BounceMatrix::plug_vec_to_row): row_input is invalid, " <<
				"won't do anything " << "(input: " << row_number <<
				" num_rows: " << num_rows << ")" << endl;
		}
		else {
			glm::mat3x2::row_type row_input;
			LINECOEFF coeff = loc_vector.get_line_coeff();
			row_input[0] = coeff.A;
			row_input[1] = coeff.B;
			row_input[2] = coeff.C;
			bounce_matrix = glm::row(bounce_matrix, row_number, row_input);
		}
	}

	void setup_matrix(LocVector input_vec, LocVector target_line_vec) {
		plug_vec_to_row(input_vec, 0);
		plug_vec_to_row(target_line_vec, 1);
	}

	void print_matrix() {
		for (int row = 0; row < num_rows; row++) {
			for (int column = 0; column < num_cols; column++) {
				cout << bounce_matrix[column][row] << " ";
			}
			cout << endl;
		}
	}

	//
	//ref/rref stuff
	
	void switch_rows(int first_row_index, int second_row_index) {
		if (!dim_input_is_valid(first_row_index, num_rows) ||
			!dim_input_is_valid(second_row_index, num_rows)) {
			cerr << "Error (in BounceMatrix::switch_rows): dimension input invalid, won't do anything " << endl << 
				"first_row_index " << first_row_index << ", second_row_index " << second_row_index << endl <<
				"number of rows" << num_rows << endl;
		}
		else {
			glm::mat3x2::row_type original_first_row = glm::row(bounce_matrix, first_row_index);
			glm::mat3x2::row_type original_second_row = glm::row(bounce_matrix, second_row_index);

			bounce_matrix = glm::row(bounce_matrix, second_row_index, original_first_row);
			bounce_matrix = glm::row(bounce_matrix, first_row_index, original_second_row);
		}
	}

	void add_scaled_row(int target_row_index, float scalar, int scaling_row_index) {
		if (!dim_input_is_valid(target_row_index, num_rows) ||
			!dim_input_is_valid(scaling_row_index, num_rows)) {
			cerr << "Error (in BounceMatrix::add_scaled_row): dimension input invalid, won't do anything " << endl <<
				"target_row_index " << target_row_index << ", scaling_row_index " << scaling_row_index << endl <<
				"number of rows " << num_rows << endl;
		}
		else {
			glm::mat3x2::row_type target_row = glm::row(bounce_matrix, target_row_index);
			glm::mat3x2::row_type original_scaling_row = glm::row(bounce_matrix, scaling_row_index);

			target_row += scalar * original_scaling_row;
			bounce_matrix = glm::row(bounce_matrix, target_row_index, target_row);
		}
	}

	//scale a row by any float, reports error w/ invalid row index
	void scale_row(int target_row_index, float scalar) {
		if (!dim_input_is_valid(target_row_index, num_rows)) {
			cerr << "Error (in BounceMatrix::scale_row): dimension input invalid, won't do anything " << endl <<
				"target_row_index " << target_row_index << endl <<
				"number of rows " << num_rows << endl;
		}
		else {
			glm::mat3x2::row_type target_row = glm::row(bounce_matrix, target_row_index);
			target_row *= scalar;

			bounce_matrix = glm::row(bounce_matrix, target_row_index, target_row);
		}
	}

	//given a col, start at row index starting_index, and find index that has
	//nonzero entry
	//this should be used in REF at stage where you know there's a nonzero entry available
	//will throw warning when that's not the case
	int find_rel_nonzero_index_in_col(glm::mat3x2::col_type col, int starting_index) {
		if (!dim_input_is_valid(starting_index, num_rows)) {
			cerr << "Error (in BounceMatrix::find_nonzero_index_in_col): starting_index should be less than num_rows, " << 
				"returning -1" << endl <<
				"starting_index " << starting_index << " num rows " << num_rows << endl;
			return -1;
		}
		else {
			int res = -1;
			bool success = false;
			for (int i = starting_index; i < num_rows; i++) {
				if (col[i] != 0.0f) {
					res = i;
					success = true;
					break;
				}
			}

			if (!success) {
				cerr << "Warning (in BounceMatrix::find_nonzero_index_in_col): didn't find any indices w/ nonzero entries" <<
					endl << ", though this function is meant to be for cases in REF where you find one." << endl << 
					" Returning -1" << endl;
			}
			return res; //note, a success could have res as -1 too

		}
	}

	//searches and returns first column index in row that is nonzero
	//used mainly in the rref function, supposed to be used only in successful cases
	//for this project
	//but will return -1 with a warning message w/ unsuccessful searches
	int find_nonzero_index_in_row(glm::mat3x2::col_type row) {
		int res = -1;
		bool success = false;
		for (int i = 0; i < num_rows; i++) {
			if (row[i] != 0.0f) {
				res = i;
				success = true;
				break;
			}
		}

		if (!success) {
			cerr << "Warning (in BounceMatrix::find_nonzero_index_in_row): didn't find any indices w/ nonzero entries" <<
				endl << ", though this function is meant to be for cases in REF where you find one." << endl <<
				" Returning -1" << endl;
		}
		return res; //note, a success could have res as -1 too
	}

	//run rref form of the 2 row 3 col matrix
	//should work for all forms of 2x3 matrices
	//note: glfw says it backwords for column major order, 2 rows, 3 cols -> 3x2 matrix
	void run_rref() {
		//
		//cout << "\n\ndoing REF" << endl;
		//

		//REF SHOULD START HERE
		int col_depth_search = 0;
		if (num_rows <= num_cols) {
			col_depth_search = num_rows - 1;
		}
		else {
			col_depth_search = num_cols;
		}

		for (int i = 0; i < col_depth_search; i++) {
			bool col_success = false;
			glm::mat3x2::col_type nonzero_col;
			int nonzero_col_index = 0;
			find_rel_nonzero_col(nonzero_col, nonzero_col_index, col_success, i, i);

			if (col_success) {
				if (nonzero_col[i] == 0) {
					int nonzero_row_index = find_rel_nonzero_index_in_col(nonzero_col, i);
					switch_rows(i, nonzero_row_index);
					nonzero_col = glm::column(bounce_matrix, nonzero_col_index); //because of switch
					  //need to update nonzero col being analyzed
				} //switch rows if needed

				float pivot_entry = nonzero_col[i];
				for (int j = (i + 1); j < num_rows; j++) {
					glm::mat3x2::row_type row = glm::row(bounce_matrix, j);
					float rel_col_entry = row[i];
					float canceling_scalar = -(rel_col_entry / pivot_entry); //this assumes pivot entry isn't 0
																			 //it shouldn't be 0 by this point, w/ proper processing
					add_scaled_row(j, canceling_scalar, i);
				}
			}
			else {
				//no nonzero columns found
				break;
			}

		}
		//
		//cout << "ref end" << endl;
		//print_matrix();
		//

		//
		//cout << "\n\nscaling to 1" << endl;
		//

		glm::mat3x2::row_type zero_row; //auto-zeroed
		for (int i = 0; i < num_rows; i++) {
			glm::mat3x2::row_type row = glm::row(bounce_matrix, i);

			if (row != zero_row) {
				int nonzero_col_index = find_nonzero_index_in_row(row);
				float index_entry = row[nonzero_col_index];
				scale_row(i, 1 / index_entry);
			}
		}
		//
		//print_matrix();
		//

		//
		//cout << "\n\nadd rows to make sure each leading entry is only nonzero entry in column" << endl;
		//
		for (int i = num_rows - 1; i > 0; i--) {
			glm::mat3x2::row_type row = glm::row(bounce_matrix, i);

			if (row != zero_row) {
				int nonzero_col_index = find_nonzero_index_in_row(row);
				float index_entry = row[nonzero_col_index];

				for (int j = i - 1; j >= 0; j--) {
					glm::mat3x2::row_type row_to_change = glm::row(bounce_matrix, j);
					int nonzero_col_index_change = find_nonzero_index_in_row(row_to_change);
					float index_entry_change = row_to_change[nonzero_col_index];

					float change_scalar = -(index_entry_change / index_entry);
					add_scaled_row(j, change_scalar, i);
				}
			}
		}
		//
		//cout << "\n\nend result" << endl;
		//print_matrix();
		//
	}

	//find relatively nonzero columns like so:
	//start finding columns from start_col_index
	//determine if column is "relatively nonzero" by starting checks from
	//start_row_index and working from there (ignore lower indices)
	//update nonzero_col on success, leave alone on failure
	void find_rel_nonzero_col(glm::mat3x2::col_type& nonzero_col, int& col_index, bool& col_success, 
		int start_row_index, int start_col_index) {
		
		bool success = false;
		for (int i = start_col_index; i < num_cols; i++) {
			glm::mat3x2::col_type current_col = glm::column(bounce_matrix, i);
			if (is_rel_nonzero_col(current_col, start_row_index)) {
				nonzero_col = current_col;
				col_index = i;
				success = true;
				break;
			}
		}

		col_success = success;
	}

	bool is_rel_nonzero_col(glm::mat3x2::col_type col, int start_row_index) {
		bool is_rel_nonzero = false;
		for (int i = start_row_index; i < num_rows; i++) {
			if (col[i] != 0.0f) {
				is_rel_nonzero = true;
				break;
			}
		}
		return is_rel_nonzero;
	}

	//assume rref already ran
	//only for 2 row 3 col case here, pretty sure this isn't
	//a general check
	bool rref_has_unique_solution() {
		bool has_unique_sol = true;
		for (int i = 0; i < num_rows; i++) {
			if (bounce_matrix[i][i] == 0) {
				has_unique_sol = false;
				break;
			}
		}

		return has_unique_sol;
	}

	//assume rref already ran
	POINT get_rref_solution() {
		POINT res_point;

		glm::mat3x2::col_type solution_col = glm::column(bounce_matrix, 2);
		res_point.x = solution_col.x;
		res_point.y = solution_col.y;

		return res_point;
	}

};

const POINT fail_point = { -1.0f , -1.0f };

class BounceProcessor {
private:
	BounceMatrix bounce_matrix;
	LocVector input_vec;
	LocVector target_line_vec;

public:
	BounceProcessor() {

	}

	void set_input_vec(LocVector input_vec) {
		this->input_vec = input_vec;
	}

	void set_target_vec(LocVector target_line_vec) {
		this->target_line_vec = target_line_vec;
	}

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
	POINT process_solve() {
		if (input_vec.has_undefined_slope() && target_line_vec.has_undefined_slope()) {
			cout << "Warning (in BounceProcessor::process_solve): both line vectors are vertical, " <<
				endl << "no unique solution possible, returning {-1,-1}" << endl;
			return fail_point;
		}
		else {
			if (input_vec.has_undefined_slope()) {
				//return y intercept of target_line_vec relative to input_vec line
				POINT y_int_point;
				y_int_point.x = input_vec.get_x();
				y_int_point.y = target_line_vec.get_y_intercept();
				return y_int_point;
			}
			else if (target_line_vec.has_undefined_slope()) {
				//return y intercept of input_vec line relative to target_line_vec
				POINT y_int_point;
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



	void print_out_bounce_matrix() {
		bounce_matrix.print_matrix();
	}


};


//
//
//this code section is written for selecting a point within a box w/
//lower left corner at (0,0) and top right corner at (1,0)

const float window_min_x = 0.0f;
const float window_max_x = 1.0f;
const float window_min_y = 0.0f;
const float window_max_y = 1.0f;

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

POINT select_exclusive_point(float window_min_x, float window_max_x,
	float window_min_y, float window_max_y) {
	bool success = false;
	POINT res;

	res.x = select_exclusive_value(window_min_x, window_max_x, "X coordinate");
	res.y = select_exclusive_value(window_min_y, window_max_y, "Y coordinate");

	return res;
};

POINT select_window_point(const char * point_description) {
	cout << "\n\nBeginning point selection for (" << point_description << ")" << endl;
	return select_exclusive_point(window_min_x, window_max_x, window_min_y,
		window_max_y);
}

POINT select_different_window_point(const char * point_description, POINT other_point) {
	POINT res_point; //don't need to initialize, do-while will ensure one
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
	POINT vector_tail = select_window_point("Vector Tail");
	POINT vector_tip = select_different_window_point("Vector Tip", vector_tail);

	float dirx = vector_tip.x - vector_tail.x;
	float diry = vector_tip.y - vector_tail.y;
	InterpolatedLV res_vector(vector_tail.x, vector_tail.y, dirx, diry);
	return res_vector;
}

//
//
//this code section is mainly for finding the target line and point an input vector
//should consider if it wants to interpolate towards a given point and then run a
//reflection afterwards


#define BOTTOM_LEFT_X 0.0f
#define BOTTOM_LEFT_Y 0.0f
#define TOP_RIGHT_X 1.0f
#define TOP_RIGHT_Y 1.0f

#define TOP_LEFT_X BOTTOM_LEFT_X
#define TOP_LEFT_Y TOP_RIGHT_Y
#define BOTTOM_RIGHT_X TOP_RIGHT_X
#define BOTTOM_RIGHT_Y BOTTOM_LEFT_Y

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

//given a FLATNESS enumerator that reflects which way a an input LocVector is pointing,
//return the window edge LocVector that the input LocVector would be pointing to
//if the flatness is NONE or odd value, write a warning and return a LocVector of zeros
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
		cout << "Warning (in LineProcessor::find_target_lines): line flatness is NONE, " << endl
			<< "returning a LocVector of all 0s" << endl;
		LocVector zeroed_vec(0.0f, 0.0f, 0.0f, 0.0f);
		return zeroed_vec;
	}
	}
}

//given what's assumed to be one of the 4 window LocVectors
//return a vector representing a 2D normal that points inside the window
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

//uses distance formula
float get_point_distance(POINT p1, POINT p2) {
	float squared_part = pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
	return pow(squared_part, 0.5f);
}

//designed for an input line and one of the window lines,
//given an input_line and 2 target_lines
//intersect the input line with each of the 2 target lines
//find which of the intersections is closer
//then return corr. BOUNCEPOINT
//
//BOUNCEPOINT workings
//given the line which the intersection is on
//find it's bounce ("normal") vector
//return a BOUNCEPOINT struct containing the intersection point
//and the bounce vector
BOUNCEPOINT find_closer_bpoint(LocVector input_line, LocVector target_line1, LocVector target_line2) {
	BOUNCEPOINT res_bp;

	BounceProcessor bp1;
	bp1.set_input_vec(input_line);
	bp1.set_target_vec(target_line1);
	POINT point1 = bp1.process_solve();

	BounceProcessor bp2;
	bp2.set_input_vec(input_line);
	bp2.set_target_vec(target_line2);
	POINT point2 = bp2.process_solve();

	POINT input_tail = input_line.get_tail();
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

//given an input_line, flat or not
//find which direction the line is pointing towards,
//find an intersection point with one of the window lines that would be made
//if the input_line were to go in the direction it's pointing towards
//then return corresponding BOUNCEPOINT
//
//BOUNCEPOINT workings
//given the line which the intersection is on
//find it's bounce ("normal") vector
//return a BOUNCEPOINT struct containing the intersection point
//and the bounce vector
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



#include <vector>

typedef double Float;

int main()
{
	/*
	std::vector<POINT> v;
	v.push_back({ -1.0f, -1.0f });
	v.push_back({ -1.0f, -.5f });
	v.push_back({ -1.0f, 0.0f });
	v.push_back({ -1.0f, 0.5f });
	v.push_back({ -1.0f, 1.0f });

	v.push_back({ -.5f, 1.0f });
	v.push_back({ 0.0f, 1.0f });
	v.push_back({ 0.5f, 1.0f });
	v.push_back({ 1.0f, 1.0f });

	v.push_back({ 1.0f, 0.5f });
	v.push_back({ 1.0f, 0.0f });
	v.push_back({ 1.0f, -0.5f });
	v.push_back({ 1.0f, -1.0f });

	v.push_back({ 0.5f, -1.0f });
	v.push_back({ 0.0f, -1.0f });
	v.push_back({ -0.5f, -1.0f });

	std::vector<POINT>::const_iterator iterator;

	for (iterator = v.begin(); iterator != v.end(); iterator++) {
		POINT poi = *iterator;
		cout << "\n\nPoint to use " << poi.x << " " << poi.y << endl;
		LocVector a(0.5f, 0.5f, poi.x, poi.y);
		BOUNCEPOINT p = find_target_bpoint(a);
		cout << "result target point: " << p.p.x << " " << p.p.y << endl;
		cout << "result bounce vector: " << p.bounce_vec.x << " " << p.bounce_vec.y << endl;
	}

	LocVector a(0.5f, 0.5f, 1.0f, 0.0f);
	BOUNCEPOINT p = find_target_bpoint(a);
	
	cout << "\n\nend result of processing" << endl;
	*/

	//LocVector start = select_contained_locvector();
	//start.report_values();

	/*
	InterpolatedLV a(0.4f, 0.4f, 0.3f, -0.1f);
	a.set_interpolate_target({ 0.9f, 0.1f });

	std::vector<float> v;
	v.push_back(0.0f);
	v.push_back(1.0f);
	v.push_back(2.5f);
	v.push_back(4.1f);
	v.push_back(5.0f);

	std::vector<float>::const_iterator iter;

	for (int i = 0; i < 4; i++) {
		cout << "\n\n Round " << i << endl;
		BOUNCEPOINT bp = find_target_bpoint(a);
		a.set_interpolate_target(bp.p);

		for (iter = v.begin(); iter != v.end(); iter++) {
			float current_iter = *iter;
			a.interpolate_tail(0.0f, 5.0f, current_iter);
			a.report_current_inter_point();
		}

		a.reflect_locvector(bp);
		a.report_values();
	}
	*/

	/*
	for (int i = 0; i < 4; i++) {
		cout << "\n\nRound " << i << endl;
		BOUNCEPOINT bp = find_target_bpoint(a);
		a.reflect_locvector(bp);
		a.report_values();
	}*/

	return 0;
}