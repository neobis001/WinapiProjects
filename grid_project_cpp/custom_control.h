#pragma once
#include "custom_window.h"

#define STARTING_CTL_IDENTIFIER 6000
#define NULL_CTL_IDENTIFIER 0

class CSControl;
typedef CSControl* PCSControl;

class CSControl : public CSWindow {
private:
	static unsigned short int available_ctl_id;

	void init_with_identifier(CSWNDEXOPS wops, PCSWindow parent);

protected:
	int ctl_identifier = NULL_CTL_IDENTIFIER;

	virtual void init(CSWNDEXOPS wops, PCSWindow parent);

public:
	CSControl();

	int get_id();

};