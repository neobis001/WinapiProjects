// crt_freopen_s.c  
// This program reassigns stderr to the file  
// named FREOPEN.OUT and writes a line to that file.  

#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <cassert>

FILE *stream;

using namespace std;

typedef struct POINT_Template {
	int x;
	int y;
} POINT;

typedef struct LINECOEFF_Template {
	float A;
	float B;
	float C;
} LINECOEFF; //meant to store floats from eq. Ax + By = C

class LocVector {
private:
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

	float get_y_intercept() {
		float slope = get_slope();
		return y - slope*x;
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
		if (input_vec.has_undefined_slope() || target_line_vec.has_undefined_slope()) {
			cerr << "Error (in BounceMatrix::setup_bounce_matrix): one of the LocVectors has " <<
				"undefined slope, this function isn't meant for those, will do nothing" <<
				endl;
		}
		else {
			plug_vec_to_row(input_vec, 0);
			plug_vec_to_row(target_line_vec, 1);
		}
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

	int find_rel_nonzero_index_in_row(glm::mat3x2::col_type row) {
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
	void run_rref() {
		cout << "\n\n\nrunning ref test" << endl;
		bounce_matrix[0][0] = 1; //r0 c1
		bounce_matrix[1][0] = 2; //r0 c2
		bounce_matrix[2][0] = 3; //r0 c3
		
		bounce_matrix[0][1] = 4; //r1 c1
		bounce_matrix[1][1] = 5; //r1 c2
		bounce_matrix[2][1] = 6; //r1 c3

		//REF SHOULD START HERE
		int col_depth_search = 0;
		if (num_rows <= num_cols) {
			col_depth_search = num_rows - 1;
		}
		else {
			col_depth_search = num_cols;
		}


		bool col_success = false;
		glm::mat3x2::col_type nonzero_col;
		find_rel_nonzero_col(nonzero_col, col_success, 1, 0);
		if (col_success) {
			cout << "nonzero col found" << endl;
			cout << nonzero_col[0] << " " << nonzero_col[1] << " " << endl;

			if (nonzero_col[0] == 0) {
				cout << "first column entry is 0" << endl;
				int nonzero_row_index = find_rel_nonzero_index_in_col(nonzero_col, 0);
				switch_rows(0, nonzero_row_index);
			} //switch rows if needed

			//now zero out bottom rows
			float pivot_entry = nonzero_col[0];
			for (int i = 1; i < num_rows; i++) {
				glm::mat3x2::row_type row = glm::row(bounce_matrix, i);
				float rel_col_entry = row[0];
				float canceling_scalar = -(rel_col_entry / pivot_entry); //this assumes pivot entry isn't 0
				  //it shouldn't be 0 by this point, w/ proper processing
				add_scaled_row(i, canceling_scalar, 0);
			}
			print_matrix();


			//END OF MAIN REF ALGORITHM

			//scale all rows to 1
			glm::mat3x2::row_type zero_row; //auto-zeroed
			for (int i = 0; i < num_rows; i++) {
				glm::mat3x2::row_type row = glm::row(bounce_matrix, i);

				if (row != zero_row) {
					int nonzero_col_index = find_rel_nonzero_index_in_row(row);
					float index_entry = row[nonzero_col_index];
					scale_row(i, 1 / index_entry);
				}
			}
			print_matrix();

			//add rows to make sure each leading entry is only nonzero entry in column
			for (int i = num_rows - 1; i > 0; i--) {
				glm::mat3x2::row_type row = glm::row(bounce_matrix, i);

				if (row != zero_row) {
					int nonzero_col_index = find_rel_nonzero_index_in_row(row);
					float index_entry = row[nonzero_col_index];
					
					for (int j = i - 1; j >= 0; j--) {
						glm::mat3x2::row_type row_to_change = glm::row(bounce_matrix, j);
						int nonzero_col_index_change = find_rel_nonzero_index_in_row(row_to_change);
						float index_entry_change = row_to_change[nonzero_col_index];

						float change_scalar = -(index_entry_change / index_entry);
						add_scaled_row(j, change_scalar, i);
					}
				}
			}
			cout << "\n\nend result:" << endl;
			print_matrix();

		}
		else {
			cout << "no nonzero cols found" << endl;
		}




		cout << "\n\n\nother test " << endl;
		print_matrix();
		glm::mat3x2::col_type t(0.0f, 2.0f);
		cout << "index is " << find_rel_nonzero_index_in_col(t, 0) << endl;
		print_matrix();
	}

	//find relatively nonzero columns like so:
	//start finding columns from start_col_index
	//determine if column is "relatively nonzero" by starting checks from
	//start_row_index and working from there (ignore lower indices)
	//update nonzero_col on success, leave alone on failure
	void find_rel_nonzero_col(glm::mat3x2::col_type& nonzero_col, bool& col_success, 
		int start_row_index, int start_col_index) {
		
		bool success = false;
		for (int i = start_col_index; i < num_cols; i++) {
			glm::mat3x2::col_type current_col = glm::column(bounce_matrix, i);
			if (is_rel_nonzero_col(current_col, start_row_index)) {
				nonzero_col = current_col;
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

};


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
	//if the target line has an undefined slope
	//  if the input line has undefined slope too, then unless
	//  they're the same line, there can be no solution.
	//  
	//  else can just get y intercept to find a solution here
	void show_solve_results() {
		if (target_line_vec.has_undefined_slope()) {
			cout << "target line has undefined slope" << endl;
			if (input_vec.has_undefined_slope()) {
				cout << "Both lines are vertical, there's no solution" << endl;
			}
			else {
				cout << "y intercept result" << endl;
				cout << input_vec.get_y_intercept() << endl;
			}
		}
		else {
			cout << "doing matrix solve" << endl;
			bounce_matrix.run_rref();
		}
	}

	void setup_bounce_matrix() {
		bounce_matrix.setup_matrix(input_vec, target_line_vec);
	}



	void print_out_bounce_matrix() {
		bounce_matrix.print_matrix();
	}

};


int main(void)
{
	LocVector a(1.0f, -1.0f, 0.9f, 1.2f);
	cout << a.get_slope() << " " << a.has_undefined_slope() << endl;
	cout << "LINECOEFF" << endl;
	LINECOEFF lc = a.get_line_coeff();
	cout << lc.A << " " << lc.B << " " << lc.C << endl;

	LocVector input(1.0f, 2.0f, 1.0f, 1.0f);
	LocVector target(0.0f, 0.0f, 0.20f, 1.0f);
	BounceProcessor bm;
	bm.set_input_vec(input);
	bm.set_target_vec(target);

	bm.print_out_bounce_matrix();
	bm.setup_bounce_matrix();
	bm.print_out_bounce_matrix();

	cout << "testing solve " << endl;
	bm.show_solve_results();

	/*
	errno_t err;
	// Reassign "stderr" to "freopen.out":  
	
	err = freopen_s(&stream, "freopen.out", "w", stderr);

	if (err != 0)
		fprintf(stdout, "error on freopen\n");
	else
	{
		fprintf(stdout, "successfully reassigned\n"); fflush(stdout);
		//fprintf(stderr, "This will go to the file 'freopen.out'\n");
		cerr << "Testing";
		fclose(stream);
	}
	system("type freopen.out");

	int a = (int)'1';
	cout << a << endl;
	*/
	return 0;
}