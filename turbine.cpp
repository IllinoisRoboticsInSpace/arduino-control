#include "turbine.h"


Turbine::Turbine(int input_pin){
	pin = input_pin;
	braked = true;
	extended = false;
}

void Turbine::extend(){
	if(extended == true){
		return;
	}else{
		AnalogWrite(pin, 255);
		extended = true;
	}
}


void Turbine::retract(){
	if(extended == false || braked == false){
		return;
	}else{
		AnalogWrite(pin, 0);
		extended = false;
	}
}

void Turbine::brake(){
	if(extended == false){//can change to automatically extend the arm when calling brake
		return;
	}else{
		braked = true;
	}
}

void Turbine::turn_on(){
	if(extended == false){//can change to automatically extend the arm when calling brake
		return;
	}else{
		braked = false;
	}
}

void Turbine::get_braking_state(){
	return braked;
}