#include "turbine.h"


turbine::turbine()
{
	extended = false;

	analogWrite(ARM_SPEED, 0);
	digitalWrite(ARM_DIR, LOW);				//LOW - Retract, HIGH - Extend
	digitalWrite(ARM_BRAKE, HIGH);
}

void turbine::extendArm()
{
	if(extended == true)
		return;
	else
	{
		extended = true;

		digitalWrite(ARM_BRAKE, LOW);
		digitalWrite(ARM_DIR, HIGH);

		analogWrite(ARM_SPEED, 122);			// 50% PWM

		delay(1000);

		analogWrite(ARM_SPEED, 0);

		digitalWrite(ARM_BRAKE, HIGH);
	}
}

void turbine::retractArm()
{
	if(extended == false)
		return;
	else
	{
		extended = false;

		digitalWrite(ARM_BRAKE, LOW);
		digitalWrite(ARM_DIR, HIGH);

		analogWrite(ARM_SPEED, 122);			// 50% PWM

		delay(1000);

		analogWrite(ARM_SPEED, 0);

		digitalWrite(ARM_BRAKE, HIGH);
	}
}