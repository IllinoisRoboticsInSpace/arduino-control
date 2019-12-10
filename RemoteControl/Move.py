from ArduinoSerial import sendData, beginTransmission
import serial
import time
import atexit

ser = serial.Serial("/dev/ttyACM0",115200)
ser.flushInput()

#The next five lines allow the motors to stop once the ctrl+c command is given to abort the program.
def exit_handler():
	sendData(ser,1,0)
	sendData(ser,2,0)

atexit.register(exit_handler)

if ser.is_open:
	beginTransmission(ser)
else:
	print("Serial Closed")

square = [(100,100),(100,-100),(100,100),(100,-100),(100,100),(100,-100),(100,100),(100,-100),(0,0)] #This should create a square

for cmd in square:
	sendData(ser, 1, cmd[0]) #This sends the commands to arduino, for each motor
	sendData(ser, 2, cmd[1])



