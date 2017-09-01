#pragma once
#include "general_types.h"
#include "LocVector.h"

class BounceMatrix {
private:
	glm::mat3x2 bounce_matrix; //note matrix is initialized by glm in rref 
	const int num_cols = 3;
	const int num_rows = 2;

	bool dim_input_is_valid(int input, int dim_size);

public:
	BounceMatrix();

	void plug_vec_to_row(LocVector loc_vector, int row_number);

	void setup_matrix(LocVector input_vec, LocVector target_line_vec);

	void print_matrix();

	//
	//ref/rref stuff
	void switch_rows(int first_row_index, int second_row_index);

	void add_scaled_row(int target_row_index, float scalar, int scaling_row_index);

	//scale a row by any float, reports error w/ invalid row index
	void scale_row(int target_row_index, float scalar);

	//given a col, start at row index starting_index, and find index that has
	//nonzero entry
	//this should be used in REF at stage where you know there's a nonzero entry available
	//will throw warning when that's not the case
	int find_rel_nonzero_index_in_col(glm::mat3x2::col_type col, int starting_index);

	//searches and returns first column index in row that is nonzero
	//used mainly in the rref function, supposed to be used only in successful cases
	//for this project
	//but will return -1 with a warning message w/ unsuccessful searches
	int find_nonzero_index_in_row(glm::mat3x2::col_type row);

	//run rref form of the 2 row 3 col matrix
	//should work for all forms of 2x3 matrices
	//note: glfw says it backwords for column major order, 2 rows, 3 cols -> 3x2 matrix
	void run_rref();

	//find relatively nonzero columns like so:
	//start finding columns from start_col_index
	//determine if column is "relatively nonzero" by starting checks from
	//start_row_index and working from there (ignore lower indices)
	//update nonzero_col on success, leave alone on failure
	void find_rel_nonzero_col(glm::mat3x2::col_type& nonzero_col, int& col_index, bool& col_success,
		int start_row_index, int start_col_index);

	bool is_rel_nonzero_col(glm::mat3x2::col_type col, int start_row_index);

	//assume rref already ran
	//only for 2 row 3 col case here, pretty sure this isn't
	//a general check
	bool rref_has_unique_solution();

	//assume rref already ran
	point_tools::POINT get_rref_solution();

};