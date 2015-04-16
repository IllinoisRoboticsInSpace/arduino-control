#include "disp.h"

Disp::Disp(int input_pin){
	pin = input_pin;
	extended = false;
}

void Disp::extendDisp(){
	if(extended == true){
		return;
	}else{
		analogWrite(pin, 122); //50 % PWM
		extended = true;
	}
}

void Disp::retractDisp(){
	if(extended == false){
		return;
	}else{
		analogWrite(pin, 0);
		extended = false;
	}
}