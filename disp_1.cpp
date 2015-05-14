#include "disp.h"

disp::disp()
{
	extended = false;

	analogWrite(DISP_SPEED, 0);
	digitalWrite(DISP_DIR, LOW);		//LOW - Retract, HIGH - Extend
	digitalWrite(DISP_BRAKE, HIGH);
}

void disp::extendDisp()
{
	digitalWrite(DISP_BRAKE, LOW);
	digitalWrite(DISP_DIR, HIGH);

	analogWrite(DISP_SPEED, 255); 		//100 % PWM
}

void disp::retractDisp()
{
	digitalWrite(DISP_BRAKE, LOW);
	digitalWrite(DISP_DIR, LOW);

	analogWrite(DISP_SPEED, 255);		// 100% PWM
}

void disp::stop()
{
	analogWrite(DISP_SPEED, 0);			//Stop actuator

	digitalWrite(DISP_BRAKE, HIGH);
}