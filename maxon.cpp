#include "maxon.h"

maxon::maxon()
{
	disable = true;
	feedback = 0;

	digitalWrite(MAXON_BRAKE, HIGH);
	digitalWrite(MAXON_DISABLE, LOW);
}

void maxon::on()
{
	disable = false;

	digitalWrite(MAXON_DISABLE, HIGH);
	digitalWrite(MAXON_DIR, HIGH);

	analogWrite(MAXON_SPEED, 255);
}

void maxon::off()
{
	disable = true;

	digitalWrite(MAXON_DISABLE, LOW);
	digitalWrite(MAXON_DIR, HIGH);

	analogWrite(MAXON_SPEED, 0);
}