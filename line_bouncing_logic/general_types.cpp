#include "general_types.h"

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