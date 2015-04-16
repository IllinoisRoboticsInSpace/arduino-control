#ifndef TURBINE_H
#define TURBINE_H

class Turbine{
public:
	Turbine(int input_pin);
	void extend();
	void retract();
	void brake();
	void turn_on();
	void get_braking_state();
private:
	int pin;
	bool braked;
	bool extended;
};


#endif