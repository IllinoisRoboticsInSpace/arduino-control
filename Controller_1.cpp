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
#define SLAM_RX 	17
#define SLAM_TX 	16
#define ROBOTEQ_RX 	19
#define ROBOTEQ_TX 	18

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
//Commands
int commaIdx[4];
int idx_start;
int idx_end;
int idx;
bool data;
String str = "";

//Robot Status
float pose[10];
String _status;

//Maxon Commands
String speed_l;
String speed_r;

//Turbine LA
int dist;
int ti_double_guh_er;
int eyeore;
String robin;

//Dispensation LA: 0: DOWN, 1: TRANSIENT_DOWN, 2: UP
int bin_state;

//IMU Commands
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
    digitalWrite(A0, HIGH);		//Set the pullup resistor

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
    Serial.begin(115200);		//ODroid Serial
    serial2.begin(115200);        	//Roboteq Serial
    //serial3.begin(115200);	  	//SLAM Serial

    //I2C communications
    Wire.begin();

    idx_start = 0;
    idx_end = 0;

    /*
    //Initialize IMU
    imu.init();
    t0 = 0;
    t1 = 0;
    delta = 0;
    */
    
    ti_double_guh_er = 0;
    eyeore = 0;
    
    bin_state = 0;

    Serial.println("Ready");
}

void loop()
{
    //Wait for data to arrive (Format: "spd_l,spd_r,paddle_LA,bin_LA,maxon#!")
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
            Serial.flush();
        }
    }

    if(data)
    {
        //Find the index of the commas
	commaIdx[0] = str.indexOf(',', idx_start);
	commaIdx[1] = str.indexOf(',', commaIdx[0]+1);
	commaIdx[2] = str.indexOf(',', commaIdx[1]+1);
	commaIdx[3] = str.indexOf(',', commaIdx[2]+1);

	//Handle left motor commands
	speed_l = myRoboteq.roboteq_set_speed_left(str.substring(idx_start, commaIdx[0]));
        serial2.write(speed_l.c_str(), speed_l.length());
        serial2.flush();

	//Handle right motor commands
	speed_r = myRoboteq.roboteq_set_speed_right(str.substring(commaIdx[0]+1, commaIdx[1]));
    	serial2.write(speed_r.c_str(), speed_r.length());
    	serial2.flush();

        //TEST TEST TEST TEST
        ti_double_guh_er = millis();
        if((ti_double_guh_er - eyeore) > 50)
        {
            robin = "\n";
            robin += String(analogRead(A14));
            robin += "\n";
            Serial.write(robin.c_str(), robin.length());
            Serial.flush();
        }
        eyeore = millis();
        
	//Handle paddle LA commands
	if(str[commaIdx[2]-1] == '0')
	{
	     dist = analogRead(A14);
            /*
	    //Potentiometer stop check
	    if(dist > 190)
	    {
	    	//Actuator fully extended
	    	myTurbine.stop();
                Serial.write("Turbine extended");
                Serial.flush();
	    }
	    else
	    {
	    	myTurbine.extendArm();
	    }*/
            myTurbine.extendArm();

	}
	else if(str[commaIdx[2]-1] == '1')
	{
	    myTurbine.stop();
	}
	else if(str[commaIdx[2]-1] == '2')
	{
	    dist = analogRead(A14);

	    //Potentiometer stop check
	    if(dist < 140)
	    {
	        myTurbine.stop();
                Serial.write("Turbine retracted");
                Serial.flush();
	    }
	    else
	    {
	        myTurbine.retractArm();
	    }
	}

	//Handle dispensation LA commands
	if(str[commaIdx[3]-1] == '0')
	{
            if(analogRead(A0) < 100)
	    {
	        if(bin_state == 1)
		{
		    //Hit limit switch
		    bin_state = 0;
		    //Serial.write("Bin down");
		    //Serial.flush();
		    myDisp.stop();
		}
		else if(bin_state == 0)
		{
		    myDisp.stop();
                    //Serial.write("Bin down");
                    //Serial.flush();
		}
		else
		{
		    //Erraneous results
		    //Serial.write("Erraneous limit switch result");
                    //Serial.flush();
		    myDisp.retractDisp();
		}

	    }
	    else
	    {
                bin_state = 1;
	        myDisp.retractDisp();
	    }

	/*
        if(analogRead(A0) < 100)
        {
            //Hit limit switch
            bin_state = 0;
            if(bin_state == false)
            {
                myDisp.stop();
            	Serial.write("Bin state: Down");
            }
        }
        else
        {
            myDisp.retractDisp();
        }

	//myDisp.retractDisp();
	*/
	}
	else if(str[commaIdx[3]-1] == '1')
	{
	    myDisp.stop();
	}
	else if(str[commaIdx[3]-1] == '2')
	{
	    myDisp.extendDisp();
	    bin_state = 2;
	}

	//Handle maxon commands
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

    /*Get IMU data
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

    //Update the robot status
    //Format: "x, y, theta, x_error, y_error, theta_error, v_x, v_theta, disp_act, turbine_act, maxon"
    pose[0] = values[0];
    pose[1] = values[1];
    pose[2] = values[3];
    pose[3] = 0;
    pose[4] = 0;
    pose[5] = 0;
    pose[6] = 0;
    pose[7] = str[commaIdx[3]-1];
    pose[8] = str[commaIdx[2]-1];
    pose[9] = str[str.length()-2];

    /*
    //Create the string to send
    _status += String(pose[0]);
    _status += ",";
    _status += String(pose[1]);
    _status += ",";
    _status += String(pose[2]);
    _status += ",";
    _status += String(pose[3]);
    _status += ",";
    _status += String(pose[4]);
    _status += ",";
    _status += String(pose[5]);
    _status += ",";
    _status += String(pose[6]);
    _status += ",";
    _status += String(pose[7]);
    _status += ",";
    _status += String(pose[8]);
    _status += ",";
    _status += String(pose[9]);
    _status += "#!";
    Serial.write(_status.c_str(), _status.length());
    Serial.flush();
    _status = "";*/
}