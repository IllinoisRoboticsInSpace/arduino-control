#include "Roboteq.h"
#include "maxon.h"
#include "disp.h"
#include "turbine.h"

//Data masks for serial communication
#define PADDLE_MASK 0x00000001
#define BIN_MASK 0x00000010
#define MAXON_MASK 0x00000100

//Call the constructors
roboteq myRoboteq = roboteq();
maxon myMaxon = maxon();
disp myDisp = disp();
turbine myTurbine = turbine();

//Global variables
//int packet_0, packet_1, packet_2;
char packet[5];

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

	//pinMode(13, OUTPUT);

	//Serial communications
	Serial.begin(9600);		//ODroid Serial
	Serial2.begin(115200);		//Roboteq Serial
}

void loop()
{
	/*while(Serial.available() > 0)
	{
		packet_0 = Serial.read();
		packet_1 = Serial.read();
		packet_2 = Serial.read();

		if(String(packet_0) == "a")
		{
			if(String(packet_1) == "b")
			{
				if(String(packet_2) == "c")
				{
					digitalWrite(13, HIGH);
					delay(10000);
					digitalWrite(13, LOW);
				}
			}
		}
	}*/
	//Wait for data to arrive
	if(Serial.available() > 0)
	{
		//Read packets
		//packet_0 = Serial.read();
		//packet_1 = Serial.read();
		//packet_2 = Serial.read();
		Serial.readBytes(packet, 5);
		Serial.print("ACK sig\n");

		//Packet_0 holds linear velocity
		myRoboteq.roboteq_set_speed_left(packet[0]);
		

		//Packet_1 holds angular velocity
		myRoboteq.roboteq_set_speed_right(packet[1]);

		//Parse packet_3
		if(packet[2] == '0')
		{
			myTurbine.extendArm();
		}
		else if(packet[2] == '1')
		{
			myTurbine.retractArm();
		}

		if(packet[3] == '1')
		{
			myDisp.extendDisp();
		}
		else fi(packet[3] == '0')
		{
			myDisp.retractDisp();
		}

		if(packet[4] == '1')
		{
			myMaxon.on();
		}
		else if(packet[4] == '0')
		{
			myMaxon.off();
		}
		/*
		if((packet[2] & PADDLE_MASK) == 0x00000001)
		{
			myTurbine.extendArm();
			Serial.println("ExtendArm");
		}
		else if((packet[2] & PADDLE_MASK) == 0)
		{
			myTurbine.retractArm();
			Serial.println("RetractArm");
		}

		if((packet[2] & BIN_MASK) == 0x00000010)
		{
			myDisp.extendDisp();
			Serial.println("ExtendDisp");
		}
		else if((packet[2] & BIN_MASK) == 0)
		{
			myDisp.retractDisp();
			Serial.println("RetractDisp");
		}

		if((packet[2] & MAXON_MASK) == 0x00000100)
		{
			myMaxon.on();
			Serial.println("Maxon on");
		}
		else if((packet[2] & MAXON_MASK) == 0)
		{
			myMaxon.off();
			Serial.println("Maxon off");
		}*/
	}
}