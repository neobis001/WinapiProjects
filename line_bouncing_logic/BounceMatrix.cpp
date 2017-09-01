#include "BounceMatrix.h"

bool BounceMatrix::dim_input_is_valid(int input, int dim_size) {
	//doing this not because of considering public access
	//but for self check
	if (input >= 0 && input < dim_size) {
		return true;
	}
	else {
		return false;
	}
}

BounceMatrix::BounceMatrix() {

}

void BounceMatrix::plug_vec_to_row(LocVector loc_vector, int row_number) {
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

void BounceMatrix::setup_matrix(LocVector input_vec, LocVector target_line_vec) {
	plug_vec_to_row(input_vec, 0);
	plug_vec_to_row(target_line_vec, 1);
}

void BounceMatrix::print_matrix() {
	for (int row = 0; row < num_rows; row++) {
		for (int column = 0; column < num_cols; column++) {
			cout << bounce_matrix[column][row] << " ";
		}
		cout << endl;
	}
}

void BounceMatrix::switch_rows(int first_row_index, int second_row_index) {
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

void BounceMatrix::add_scaled_row(int target_row_index, float scalar, int scaling_row_index) {
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

void BounceMatrix::scale_row(int target_row_index, float scalar) {
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

int BounceMatrix::find_rel_nonzero_index_in_col(glm::mat3x2::col_type col, int starting_index) {
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

int BounceMatrix::find_nonzero_index_in_row(glm::mat3x2::col_type row) {
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

void BounceMatrix::run_rref() {
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

void BounceMatrix::find_rel_nonzero_col(glm::mat3x2::col_type& nonzero_col, int& col_index, bool& col_success,
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

bool BounceMatrix::is_rel_nonzero_col(glm::mat3x2::col_type col, int start_row_index) {
	bool is_rel_nonzero = false;
	for (int i = start_row_index; i < num_rows; i++) {
		if (col[i] != 0.0f) {
			is_rel_nonzero = true;
			break;
		}
	}
	return is_rel_nonzero;
}

bool BounceMatrix::rref_has_unique_solution() {
	bool has_unique_sol = true;
	for (int i = 0; i < num_rows; i++) {
		if (bounce_matrix[i][i] == 0) {
			has_unique_sol = false;
			break;
		}
	}

	return has_unique_sol;
}

point_tools::POINT BounceMatrix::get_rref_solution() {
	point_tools::POINT res_point;

	glm::mat3x2::col_type solution_col = glm::column(bounce_matrix, 2);
	res_point.x = solution_col.x;
	res_point.y = solution_col.y;

	return res_point;
}