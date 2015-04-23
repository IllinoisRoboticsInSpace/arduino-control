#include "Roboteq.h"
#include "maxon.h"
#include "disp.h"
#include "turbine.h"
#include "FreeSixIMU.h"
#include "FIMU_ADXL345.h"
#include "FIMU_ITG3200.h"
//Call the constructors
roboteq myRoboteq = roboteq();
maxon myMaxon = maxon();
float deltaX;
float deltaY;
float deltaTheta;
//http://stackoverflow.com/questions/28091151/double-integrating-acceleration-in-c-using-a-9dof-imu-with-sensor-fusion for double integrals
void setup()
{
	/* Connections:
	*		ODroid - Serial1(9600)
	*		Maxon - Analog, Digital x4
disp myDisp = disp();
turbine myTurbine = turbine();

//Global variables
char packet[5];
int data;
float accelerations[3];
int x;
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
	Serial.begin(115200);		//ODroid Serial
	Serial2.begin(115200);		//Roboteq Serial
	sixDOF.init();
	Serial.println("SIG READY");
}

void loop()
{
	//Wait for data to arrive
	if(Serial.available() == 5)
	{
		data = Serial.readBytes(packet, 5);

		if(data == 5)
		{
			//Print back the data sent
			Serial.write(packet);
			Serial.flush();
		}
		else
		{
			Serial.write("Bad data!");
			Serial.flush();
		}

		//Packet[0] holds left motor commands
		myRoboteq.roboteq_set_speed_left(packet[0]);
		

		//Packet[1] holds right motor commands
		myRoboteq.roboteq_set_speed_right(packet[1]);

		//Packet[2] holds paddle LA commands
		if(packet[2] == '0')
		{
			myTurbine.extendArm();
		}
		else if(packet[2] == '1')
		{
			myTurbine.retractArm();
		}

		//Packet[3] holds dispensation LA commands
		if(packet[3] == '1')
		{
			myDisp.extendDisp();
		}
		else if(packet[3] == '0')
		{
			myDisp.retractDisp();
		}

		//Packet[4] holds maxon commands
		if(packet[4] == '1')
		{
			myMaxon.on();
		}
		else if(packet[4] == '0')
		{
			myMaxon.off();
		}
	}
}