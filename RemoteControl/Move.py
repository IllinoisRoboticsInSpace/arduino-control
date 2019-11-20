from ArduinoSerial import sendData
import serial
import time
import atexit

ser = serial.Serial("/dev/ttyACM0",9600)
ser.flushInput()


#The next five lines allow the motors to stop once the ctrl+c command is given to abort the program.
def exit_handler():
	sendData(ser,1,0)
	sendData(ser,2,0)

atexit.register(exit_handler)

square = [(100,100),(100,-100),(100,100),(100,-100),(100,100),(100,-100),(100,100),(100,-100),(0,0)] #This should create a square

sendData(ser,2,0) #This was a test intialization command, can be removed
time.sleep(1)

for cmd in square:
	sendData(ser, 1, cmd[0]) #This sends the commands to arduino, for each motor
	sendData(ser, 2, cmd[1])




