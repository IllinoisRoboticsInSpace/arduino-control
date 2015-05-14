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
	digitalWrite(ARM_BRAKE, LOW);
	digitalWrite(ARM_DIR, HIGH);

	analogWrite(ARM_SPEED, 122);			// 50% PWM
}

void turbine::retractArm()
{
	digitalWrite(ARM_BRAKE, LOW);
	digitalWrite(ARM_DIR, LOW);

	analogWrite(ARM_SPEED, 122);			// 50% PWM
}

void turbine::stop()
{
	analogWrite(ARM_SPEED, 0);				//Stop actuator

	digitalWrite(ARM_BRAKE, HIGH);
}