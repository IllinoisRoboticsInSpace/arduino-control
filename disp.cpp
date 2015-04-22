#include "disp.h"

disp::disp()
{
	extended = false;

	analogWrite(DISP_SPEED, 0);
	digitalWrite(DISP_DIR, LOW);			//LOW - Retract, HIGH - Extend
	digitalWrite(DISP_BRAKE, HIGH);
}

void disp::extendDisp()
{
	if(extended == true)
		return;
	else
	{
		extended = true;

		digitalWrite(DISP_BRAKE, LOW);
		digitalWrite(DISP_DIR, HIGH);

		analogWrite(DISP_SPEED, 122); 		//50 % PWM

		delay(1000);

		analogWrite(DISP_SPEED, 0);

		digitalWrite(DISP_BRAKE, HIGH);
	}
}

void disp::retractDisp()
{
	if(extended == false)
		return;
	else
	{
		extended = false;

		digitalWrite(DISP_BRAKE, LOW);
		digitalWrite(DISP_DIR, LOW);

		analogWrite(DISP_SPEED, 122);		// 50% PWM

		delay(1000);

		analogWrite(DISP_SPEED, 0);

		digitalWrite(DISP_BRAKE, HIGH);
	}
}