#include <SoftwareSerial.h>
#include <Wire.h>
#include "Roboteq.h"
#include "turbine.h"
#include "disp.h"
#include "maxon.h"


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

//Global variables
//Commands
int commaIdx[4];
int idx_start;
int idx_end;
int idx;
bool data;
String str = "";

//Turbine LA
int dist;
int ti_double_guh_er;
int eyeore;
String robin;

//Dispensation LA: 0: DOWN, 1: TRANSIENT_DOWN, 2: UP
int bin_state;

//Maxon Commands
String speed_l;
String speed_r;

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

    //Paddle LA pinout
    pinMode(ARM_SPEED, OUTPUT);
    pinMode(ARM_DIR, OUTPUT);
    pinMode(ARM_BRAKE, OUTPUT);
    pinMode(ARM_THERMAL, INPUT);
    pinMode(ARM_POT, INPUT);
    pinMode(ARM_CURRENT, INPUT);

    //Dispensation LA pinout
    pinMode(DISP_SPEED, OUTPUT);
    pinMode(DISP_DIR, OUTPUT);
    pinMode(DISP_BRAKE, OUTPUT);
    pinMode(DISP_THERMAL, INPUT);
    pinMode(DISP_HALL, INPUT);
    pinMode(DISP_POT, INPUT);
    pinMode(DISP_CURRENT, INPUT);
    digitalWrite(A0, HIGH);		//Set the pullup resistor

    //Maxon pinout
    pinMode(MAXON_SPEED, OUTPUT);
    pinMode(MAXON_DIR, OUTPUT);
    pinMode(MAXON_BRAKE, OUTPUT);
    pinMode(MAXON_DISABLE, OUTPUT);
    pinMode(MAXON_HALL, INPUT);

    //SLAM serial port
    pinMode(SLAM_RX, INPUT);
    pinMode(SLAM_TX, OUTPUT);
        
    //Roboteq serial port        
    pinMode(ROBOTEQ_RX, INPUT);
    pinMode(ROBOTEQ_TX, OUTPUT);
        
    //Serial communications
    Serial.begin(115200);			//ODroid Serial
    serial2.begin(115200);        	//Roboteq Serial
    //serial3.begin(115200);	  	//SLAM Serial

    //I2C communications
    Wire.begin();

    //Init parse vars
    idx_start = 0;
    idx_end = 0;

    //Init turbine limits and state
    ti_double_guh_er = 0;
    eyeore = 0;
    bin_state = 0;

    //Setup complete
    delay(10);
    Serial.write("Ready");
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

        /*TEST TEST TEST TEST*/
        ti_double_guh_er = millis();
        if((ti_double_guh_er - eyeore) > 500)
        {
            robin = String(analogRead(A14));
            Serial.write(robin.c_str(), robin.length());
            Serial.flush();
        }
        eyeore = millis();
        
		//Handle paddle LA commands
		if(str[commaIdx[2]-1] == '0')
		{
	    	dist = analogRead(A14);

	    	//Potentiometer max is 3.3V, so only use 2/3 of range (682)
	    	if(dist > 382)
	    	{
	    		//Actuator fully extended
            	Serial.write("Turbine extended");
            	Serial.flush();
	    	}
	    	else
	    	{
	    		myTurbine.extendArm();
	    	}

		}
		else if(str[commaIdx[2]-1] == '1')
		{
	    	myTurbine.stop();
		}
		else if(str[commaIdx[2]-1] == '2')
		{
		    dist = analogRead(A14);

		    //Potentiometer max is 3.3V, so only use 2/3 of range (682)
	    	if(dist < 100)
	    	{
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
		    		myDisp.stop();
		    		bin_state = 0;
				}
				else if(bin_state == 0)
				{
		    		myDisp.stop();
				}
				else
				{
		    		//Erraneous results, ignore
		    		myDisp.retractDisp();
				}
	    	}
	    	else
	    	{
            	bin_state = 1;
	        	myDisp.retractDisp();
	    	}
		}
		else if(str[commaIdx[3]-1] == '1')
		{
	    	myDisp.stop();
		}
		else if(str[commaIdx[3]-1] == '2')
		{
			bin_state = 2;
	    	myDisp.extendDisp();
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

		//Reset data read for next iteration
        data = false;
    }
}