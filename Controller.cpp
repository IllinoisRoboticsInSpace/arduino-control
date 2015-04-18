#include "Roboteq.h"
#include "maxon.h"
#include "disp.h"
#include "turbine.h"

//Data masks for serial communication
#define PADDLE_MASK 0x10000000
#define BIN_MASK 0x01000000
#define MAXON_MASK 0x00100000

//Call the constructors
roboteq roboteq();
maxon maxon();
disp disp();
turbine turbine();

//Global variables
int packet_0, packet_1, packet_2;

void setup()
{
	/* Connections:
	*		ODroid - Serial1(9600)
	*		Maxon - Analog, Digital x4
	*		Roboteq - Serial(115200), Analog, Digital
	*		Disp. LA - Digital(PWM), Digital x4, Analog x2
	*		Tubine LA - Digital(PWM), Digital x4, Analog x2
	*/

	//Roboteq pinout
	pinMode(ROBOTEQ_SPEED, OUTPUT);
	pinMode(ROBOTEQ_DIR, OUTPUT);
	pinMode(ROBOTEQ_BRAKE, OUTPUT);

	//Maxon pinout
	pinMode(MAXON_SPEED, OUTPUT);
	pinMode(MAXON_DIR, OUTPUT);
	pinMode(MAXON_BRAKE, OUTPUT);
	pinMode(MAXON_DISABLE, OUTPUT);
	pinMode(MAXON_HALL, INPUT);

	//Dispensation LA pinout
	pinMode(DISP_SPEED, OUTPUT);
	pinMode(DISP_DIR, OUTPUT);
	pinMode(DISP_BRAKE, OUTPUT);
	pinMode(DISP_THERMAL, INPUT);
	pinMode(DISP_HALL, INPUT);
	pinMode(DISP_POT, INPUT);
	pinMode(DISP_CURRENT, INPUT);

	//Paddle LA pinout
	pinMode(ARM_SPEED, OUTPUT);
	pinMode(ARM_DIR, OUTPUT);
	pinMode(ARM_BRAKE, OUTPUT);
	pinMode(ARM_THERMAL, INPUT);
	pinMode(ARM_POT, INPUT);
	pinMode(ARM_CURRENT, INPUT);

	//Serial communications
	Serial1.begin(9600);		//ODroid Serial
	Serial2.begin(115200);		//Roboteq Serial
}

void loop()
{
	//Wait for data to arrive
	if(Serial1.available() > 0)
	{
		//This is a test segment
		Serial1.write(Serial1.read());

		/*
		//Read packets
		packet_0 = Serial1.read();
		packet_1 = Serial1.read();
		packet_2 = Serial1.read();

		//Packet_0 holds linear velocity
		roboteq_set_speed(packet_0);

		//Packet_1 holds angular velocity
		roboteq_set_direction(packet_1);

		//Parse packet_3
		if((packet_2 & PADDLE_MASK) == 0)
		{

		}
		*/
	}
}