#include "maxon.h"

maxon::maxon()
{
	pwm = 0;
	disable = false;
	on_off = false;
	feedback = 0;
}

maxon::on()
{
	disable = true;

	analogWrite(pin#, 255);
}

maxon::off()
{
	disable = false;

	analogWrite(pin#, 0);
}