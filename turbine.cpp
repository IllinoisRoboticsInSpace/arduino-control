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

		AnalogWrite(ARM_SPEED, 122);			// 50% PWM
	}
}

void turbine::extendArmStop()
{
	analogWrite(ARM_SPEED, 0);

	digitalWrite(ARM_BRAKE, HIGH);
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

		AnalogWrite(ARM_SPEED, 122);			// 50% PWM
	}
}

void turbine::retractArmStop()
{
	analogWrite(ARM_SPEED, 0);

	digitalWrite(ARM_BRAKE, HIGH);
}