#ifndef TURBINE_H
#define TURBINE_H

#include "Arduino.h"

#define ARM_SPEED 	44		// Pins
#define ARM_DIR 	48
#define ARM_BRAKE 	50
#define ARM_THERMAL 52
#define ARM_POT 	82
#define ARM_CURRENT 83

class turbine
{
public:
	turbine();
	void extendArm();
	void retractArm();
	void stop();
private:
	bool extended;
};

#endif