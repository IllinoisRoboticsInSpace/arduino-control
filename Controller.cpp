#include <SoftwareSerial.h>
#include <Wire.h>
#include "Roboteq.h"
#include "maxon.h"
#include "disp.h"
#include "turbine.h"
#include "FreeSixIMU.h"
#include "FIMU_ADXL345.h"
#include "FIMU_ITG3200.h"

//Define the serial port pins
#define SLAM_RX 	19
#define SLAM_TX 	18
#define ROBOTEQ_RX 	17
#define ROBOTEQ_TX 	16

//Define serial communications
SoftwareSerial serial1(SLAM_RX, SLAM_TX);
SoftwareSerial serial2(ROBOTEQ_RX, ROBOTEQ_TX);

//Call the constructors
roboteq myRoboteq = roboteq();
maxon myMaxon = maxon();
disp myDisp = disp();
turbine myTurbine = turbine();
FreeSixIMU imu = FreeSixIMU();

//Global variables
char packet[5];
int data;

String speed_l;
String speed_r;

float values[6];
int t0;
int t1;
int delta;

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

    //SLAM serial port
    pinMode(SLAM_RX, INPUT);
    pinMode(SLAM_TX, OUTPUT);
        
    //Roboteq serial port        
    pinMode(ROBOTEQ_RX, INPUT);
    pinMode(ROBOTEQ_TX, OUTPUT);
        
	//Serial communications
	Serial.begin(115200);	  //ODroid Serial
    serial1.begin(115200);    //SLAM Serial
	serial2.begin(115200);	  //Roboteq Serial

	//I2C communications
	Wire.begin();

	//Initialize IMU
	imu.init();
	t0 = 0;
	t1 = 0;
	delta = 0;

	Serial.write("Ready");
}

void loop()
{
	//Wait for data to arrive
	if(Serial.available() > 0)
	{
		data = Serial.readBytes(packet, 5);

		if(data > 0)
		{
			//Print back the data sent
			Serial.write(packet, 5);
			Serial.flush();
		}
		else
		{
			Serial.write("Bad data!");
			Serial.flush();
		}

		//Packet[0] holds left motor commands
		speed_l = myRoboteq.roboteq_set_speed_left(packet[0]);
        serial2.write(speed_l.c_str(), speed_l.length());
        serial2.flush();
		

		//Packet[1] holds right motor commands
		speed_r = myRoboteq.roboteq_set_speed_right(packet[1]);
        serial2.write(speed_r.c_str(), speed_r.length());
        serial2.flush();

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

	//Get IMU data
	t1 = micros()

	imu.getValues(values);
	imu.getYawPitchRoll(&values[3]);

	//Compute x, y, z positions
	delta = t1 - t0;
	values[0] *= delta * delta;
	values[1] *= delta * delta;
	values[2] *= delta * delta;

	//Normalize positions to flat plane
	values[0] *= cos(values[3]);
	values[1] *= cos(values[4]);

	//Update timestep
	t0 = micros();
}