#include "Roboteq.h"
#include "Arduino.h"
//make a roboteq object, initializing elements to 0
roboteq::roboteq()
{
	curr_speed = 0;
	curr_direction = 0;

	digitalWrite(ROBOTEQ_BRAKE, HIGH);
}	

//open port at 115200 baud rate
// int Roboteq::roboteq_init()
// {
// 	Serial1.begin(BAUD);
// }

//takes a speed from -1000 to 1000 and sets the motors
int roboteq::roboteq_set_speed_right(char c_speed)
{
	speed_r = int(c_speed);

	//Scale to range of -1000, 1000
	speed_r -= 127;
	speed_r *= 8;
	speed_r = -speed_r

	//check bounds
	if(speed_r > MAX || speed_r < MIN)
		return -1;

	digitalWrite(ROBOTEQ_BRAKE, LOW);

	//Convert speed to usable parameter and parse to string
	speed_str_r = String("!g 1 " + speed_r);

	//write go command, channel, and value
	if(Serial2.print(speed_str_r) != (5+speed_str_r.length()))
		return -1;

	//update current speed
	curr_speed_r = speed_r;

	Serial.println("curr_speed");
	return 0;
}

//takes a speed from -1000 to 1000 and sets the motors
int roboteq::roboteq_set_speed_left(char c_speed)
{
	speed_l = int(c_speed);

	//Scale to range of -1000, 1000
	speed_l -= 127;
	speed_l *= 8;

	//check bounds
	if(speed_l > MAX || speed_l < MIN)
		return -1;

	digitalWrite(ROBOTEQ_BRAKE, LOW);

	//Convert speed to usable parameter and parse to string
	speed_str_l = String("!g 2 " + speed_l);

	//write go command, channel, and value
	if(Serial2.print(speed_str_l) != (5+speed_str_l.length()))
		return -1;

	//update current speed
	curr_speed_l = speed_l;

	Serial.println("curr_speed");
	return 0;
}

//takes a direction from -1000 to 1000 and sets the motors
int roboteq::roboteq_set_direction(int direction)
{
	//check bounds
	if(direction > MAX || direction < MIN)
		return -1;

	digitalWrite(ROBOTEQ_BRAKE, LOW);

	//Convert direction to usable parameter and parse to string
	dir_str = String("!g 2 " + (direction * ANGULAR_TO_DIR)); 

	//write go command, channel, and value
	if(Serial2.print(dir_str) != (5+dir_str.length()))
		return -1;

	//update direction
	curr_direction = direction;
	Serial.println("curr_direction");
	return 0;
}

//return current speed
int roboteq::roboteq_get_speed()
{
	return curr_speed;
}

//return current direction
int roboteq::roboteq_get_direction()
{
	return curr_direction;
}

//close the port
void roboteq::roboteq_close()
{
	Serial1.end();
}
