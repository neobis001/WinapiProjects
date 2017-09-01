#pragma once
#include "general_types.h"
#include "LocVector.h"
#include "BounceProcessor.h"

//
//
//this code section is mainly for finding the target line and point an input vector
//should consider if it wants to interpolate towards a given point and then run a
//reflection afterwards
//The target line and point mentioned above are used in forming a BOUNCEPOINT struct
//which contains a different "normal" glm::vec2 and a POINT struct
//This section is mainly just for finding it and nothing else
//This doesn't involve interpolation calculations

#define BOTTOM_LEFT_X 0.0f
#define BOTTOM_LEFT_Y 0.0f
#define TOP_RIGHT_X 1.0f
#define TOP_RIGHT_Y 1.0f

#define TOP_LEFT_X BOTTOM_LEFT_X
#define TOP_LEFT_Y TOP_RIGHT_Y
#define BOTTOM_RIGHT_X TOP_RIGHT_X
#define BOTTOM_RIGHT_Y BOTTOM_LEFT_Y

//given a FLATNESS enumerator that reflects which way a an input LocVector is pointing,
//return the window edge LocVector that the input LocVector would be pointing to
//if the flatness is NONE or odd value, write an error msg and abort
LocVector find_target_line_from_flatness(FLATNESS flatness);

//given what's assumed to be one of the 4 window LocVectors
//return a vector representing a 2D normal that points inside the window
glm::vec2 get_window_bounce_vec(LocVector target_line);

//uses distance formula
float get_point_distance(point_tools::POINT p1, point_tools::POINT p2);

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
BOUNCEPOINT find_closer_bpoint(LocVector input_line, LocVector target_line1, LocVector target_line2);

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
BOUNCEPOINT find_target_bpoint(LocVector input_line);
