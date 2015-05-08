#include "Roboteq.h"
#include "Arduino.h"

//make a roboteq object, initializing elements to 0
roboteq::roboteq()
{
	speed_l = 0;
    speed_r = 0;
	dir = 0;

	//digitalWrite(ROBOTEQ_BRAKE, HIGH);
}	

//open port at 115200 baud rate
int roboteq::roboteq_init()
{
	//serial2.begin(BAUD);
}

//takes a speed from -1000 to 1000 and sets the motors
String roboteq::roboteq_set_speed_right(String speed_r)
{
	//Convert speed to usable parameter and parse to string
	speed_str_r = "!g 1 ";
    speed_str_r += speed_r;
    speed_str_r += "\r";

	//write go command, channel, and value
    //serial2.write(speed_str_r.c_str(), speed_str_r.length());

	return speed_str_r;
}

//takes a speed from -1000 to 1000 and sets the motors
String roboteq::roboteq_set_speed_left(String speed_l)
{
	//Convert speed to usable parameter and parse to string
    speed_str_l = "!g 2 ";
	speed_str_l += speed_l;
    speed_str_l += "\r";

	//write go command, channel, and value
    //serial2.write(speed_str_l.c_str(), speed_str_l.length());

	return speed_str_l;
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
	//if(serial2.print(dir_str) != (5+dir_str.length()))
	//	return -1;

	//Update direction
	dir = direction;

	return 0;
}

//return current left motor speed
int roboteq::roboteq_get_speed_right()
{
	return speed_r;

}

//return current left motor speed
int roboteq::roboteq_get_speed_left()
{
	return speed_l;

}

//return current direction
int roboteq::roboteq_get_direction()
{
	return dir;
}

//close the port
void roboteq::roboteq_close()
{
	//serial2.end();
}
