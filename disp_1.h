#ifndef DISP_H
#define DISP_H

#include "Arduino.h"

#define DISP_SPEED 		45	// Pins
#define DISP_DIR 		47
#define DISP_BRAKE 		49
#define DISP_THERMAL 	51
#define DISP_HALL 		53
#define DISP_POT 		85
#define DISP_CURRENT 	86

class disp
{
public:
	disp();
	void extendDisp();
	void retractDisp();
	void stop();
private:
	bool extended;
};

#endif