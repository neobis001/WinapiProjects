#pragma once
#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <cassert>
#include <cmath>

#include "point_tools.h"

using namespace std;

typedef struct LINECOEFF_Template {
	float A;
	float B;
	float C;
} LINECOEFF; //meant to store floats from eq. Ax + By = C

enum QUADRANT {
	Q1,
	Q2,
	Q3,
	Q4,
	QNONE
};

//
//BOUNCEPOINT section, just a definition

typedef struct BOUNCEPOINT_Template {
	glm::vec2 bounce_vec;
	point_tools::POINT p;
} BOUNCEPOINT;


//
//
//this section of code is for purely the FLATNESS
//enumeration
//the FLATNESS value represents whether a vector is on the x-axis/y-axis and which
//direction it's pointing at
//NONE means the vector isn't on the x/y axis

enum FLATNESS {
	POS_X,
	NEG_X,
	POS_Y,
	NEG_Y,
	NONE
};
