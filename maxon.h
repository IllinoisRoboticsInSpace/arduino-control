#ifndef MAXON_H
#define MAXON_H

#include "Arduino.h"

#define MAXON_SPEED 	4	// Pins
#define MAXON_DIR 		25
#define MAXON_BRAKE 	26
#define MAXON_DISABLE 	27
#define MAXON_HALL 		28

class maxon
{
public:
 	maxon();
 	void on();
 	void off();

private:
	bool disable;			// Turns motor on/off
	float feedback;
};

#endif