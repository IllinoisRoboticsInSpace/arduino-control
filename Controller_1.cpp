#include <SoftwareSerial.h>
#include <Wire.h>
#include "Roboteq.h"
#include "maxon.h"
#include "disp.h"
#include "turbine.h"
//#include "FIMU_ADXL345.h"
//#include "FIMU_ITG3200.h"
//#include "FreeSixIMU.h"

//Define the serial port pins
#define SLAM_RX 	19
#define SLAM_TX 	18
#define ROBOTEQ_RX 	17
#define ROBOTEQ_TX 	16

//Define serial communications
SoftwareSerial serial2(ROBOTEQ_RX, ROBOTEQ_TX);
SoftwareSerial serial3(SLAM_RX, SLAM_TX);

//Call the constructors
roboteq myRoboteq = roboteq();
maxon myMaxon = maxon();
disp myDisp = disp();
turbine myTurbine = turbine();
//FreeSixIMU imu = FreeSixIMU();

//Global variables
int commaIdx[4];
int idx_start;
int idx_end;
int idx;
String str = "";

bool data;

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
    serial2.begin(115200);    //Roboteq Serial
	//serial3.begin(115200);	  //SLAM Serial

	//I2C communications
	Wire.begin();

	idx_start = 0;
	idx_end = 0;
/*
	//Initialize IMU
	imu.init();
	t0 = 0;
	t1 = 0;
	delta = 0;*/

	Serial.write("Ready");
}

void loop()
{
	//Wait for data to arrive
	while(Serial.available() > 0)
	{
        str = "";
		str = Serial.readStringUntil('!');
		idx_start = 0;
		idx_end = str.length();

        // Check we got good data
        data = str.endsWith("#");
        //Print the data back
	    if (!(Serial.available() > 0))
        {
            Serial.write(str.c_str(), str.length());
            if(data)
                Serial.write("\nData: True\n");
            else
                Serial.write("\nData: False\n");
            Serial.flush();
        }
    }
    
    // Check we got good data
    //data = str.endsWith("#");
    //Print the data back
	//Serial.write(str.c_str(), str.length());
	//Serial.flush();

	if(data)
	{
		//Find the index of the commas
		commaIdx[0] = str.indexOf(',', idx_start);
		commaIdx[1] = str.indexOf(',', commaIdx[0]+1);
		commaIdx[2] = str.indexOf(',', commaIdx[1]+1);
		commaIdx[3] = str.indexOf(',', commaIdx[2]+1);

		//Packet[0] holds left motor commands
		speed_l = myRoboteq.roboteq_set_speed_left(str.substring(idx_start, commaIdx[0]));
        serial2.write(speed_l.c_str(), speed_l.length());
        serial2.flush();
        Serial.write("speed_l: ");
        Serial.write(speed_l.c_str(), speed_l.length());
        Serial.flush();
        Serial.write("Feedback from roboteq left motor: ");
        Serial.write(Serial.read());

		//Packet[1] holds right motor commands
		speed_r = myRoboteq.roboteq_set_speed_right(str.substring(commaIdx[0]+1, commaIdx[1]));
    	serial2.write(speed_r.c_str(), speed_r.length());
    	serial2.flush();
        Serial.write("speed_r: ");
        Serial.write(speed_r.c_str(), speed_r.length());
        Serial.flush();

		//Packet[2] holds paddle LA commands
		if(str[commaIdx[2]-1] == '0')
		{
			myTurbine.extendArm();
		}
		else if(str[commaIdx[2]-1] == '1')
		{
			myTurbine.stop();
		}
		else if(str[commaIdx[2]-1] == '2')
		{
			myTurbine.retractArm();
		}

		//Packet[3] holds dispensation LA commands
		if(str[commaIdx[3]-1] == '0')
		{
			myDisp.retractDisp();
		}
		else if(str[commaIdx[3]-1] == '1')
		{
			myDisp.stop();
		}
		else if(str[commaIdx[3]-1] == '2')
		{
			myDisp.extendDisp();
		}

		//Packet[4] holds maxon commands
		if(str[str.length()-2] == '1')
		{
			myMaxon.on();
		}
		else
		{
			myMaxon.off();
		}
            data = false;
	}
/*
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
	t0 = micros();*/
}