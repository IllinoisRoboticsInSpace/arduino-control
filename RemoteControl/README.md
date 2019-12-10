This folder contains the necessary files for communication between Arduino (Sabertooth) and Python (ROS).
As of 12/09/19 is contains the following files:
 1. ArduinoSerial.py
 2. Move.py
 3. RemoteManual.py
 4. RemoteManual.ino
 5. SabertoothMotor.cpp
 6. SabertoothMotor.h
 
To use these files the [pyserial](https://pythonhosted.org/pyserial/) library must be installed. This allows communication through arduino serial via python.

# ArduinoSerial.py
This file contains all the necessary functions to send data through the serial.
## sendData
this function takes the serial object, the motor number and the selected speed (from -128 to 129) and converts it and then sends it to Arduino. 
The format is #[motN]$[speed] although the speed goes from 0 to  256

## beginTransmission
Takes in a serial object and sends a "begin" command to Arduino and then awaits a "done" to make sure the Arduino is connected and communicating. 

# Move.py
This file uses the functions from ArduinoSerial.py to communicate with Arduino.
First if creates the serial object. on the ttyACM0 port. This port should be the default port. Once the connection is opened python checks that it is and if so, calls the beginTransmission function to make sure Arduino is connected and responding.
Then square is just an array that contains the power setting for each motor (1 and 2) to create a square.  The commands are then sent with the for loop.
Lastly the exit_handler function shuts off the motors in case of a forced stop of the program (e.g. ctrl + c)


TODO:
New communication protocol "[n]#[motor]$[power]" for n = 1, "[n]#[motor]$[power]#[motor]$[power]" for n = 2 etc.
- [ ] Implement new communication protocol on Python side
- [ ] Implement new communication protocol on Arduino side

