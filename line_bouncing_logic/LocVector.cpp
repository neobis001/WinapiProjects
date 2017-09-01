#include "LocVector.h"

LocVector::LocVector() {

}

LocVector::LocVector(float x, float y, float dirx, float diry) {
	this->x = x;
	this->y = y;

	if (dirx == 0.0f) {
		cerr << "Warning (in LocVector constructor): if dirx is 0, slope will be undefined, keep in mind" << endl;
	}
	this->dirx = dirx;
	this->diry = diry;
}

LINECOEFF LocVector::get_line_coeff() {
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

bool LocVector::has_undefined_slope() {
	if (dirx == 0.0f) {
		return true;
	}
	else {
		return false;
	}
}

float LocVector::get_slope() {
	if (has_undefined_slope()) {
		cerr << "Warning (in LocatedVector::get_slope): slope is undefined, returning -1" << endl;
		return -1.0f;
	}
	else {
		return diry / dirx;
	}
}

float LocVector::get_y_intercept() {
	float slope = get_slope();
	return y - slope*x;
}


float LocVector::get_x() {
	return x;
}

float LocVector::get_y() {
	return y;
}

void LocVector::report_values() {
	cout << "x: " << x << " y: " << y << " dirx: " << dirx << " diry: " << diry << endl;
}

bool LocVector::is_flat_line() {
	if (has_undefined_slope() || get_slope() == 0) {
		return true;
	}
	else {
		return false;
	}
}

FLATNESS LocVector::get_flatness() {
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

QUADRANT LocVector::get_dir_quadrant() {
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

point_tools::POINT LocVector::get_tail() {
	point_tools::POINT tail;
	tail.x = x;
	tail.y = y;
	return tail;
}

point_tools::POINT LocVector::get_tip() {
	point_tools::POINT tip;
	tip.x = x + dirx;
	tip.y = y + diry;
	return tip;
}

float LocVector::plug_in_x_for_y(float input_x) {
	LINECOEFF coeffs = get_line_coeff();
	float A = coeffs.A;
	float B = coeffs.B;
	float C = coeffs.C;

	return (-A / B)*input_x + (C / B);
}

void LocVector::reflect_locvector(BOUNCEPOINT bp) {
	glm::vec2 current_vec;
	current_vec.x = dirx;
	current_vec.y = diry;

	glm::vec2 reflected_vec = glm::reflect(current_vec, bp.bounce_vec);

	x = bp.p.x;
	y = bp.p.y;
	dirx = reflected_vec.x;
	diry = reflected_vec.y;
}

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