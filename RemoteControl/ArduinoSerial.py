#this file contains all the necessary functions for communications with arduino. 
#Eventually it will have functions to initialize comunication and any other required function

#use pip3 install pyserial
#import serial
#import time

debug = True

def sendData(ser, motN, spd):
	#ser is the serial object
	#motn is the motor number; 1-2 are for the first sabetooth, 3-4 for the second and so on...
	#spd is the speed, from 128 to -128
	speed = spd+128
	cmd = bytes("#"+str(motN)+"$"+str(speed),'ascii')
	ser.write(cmd)
	if debug: print(cmd)


def beginTransmission(ser):
	ser.write(bytes("begin\n",'ascii'))
	if debug: print("Initialization command sent\nAwaiting response...")
	print(str(ser.readline(), 'ascii'))
	if str(ser.readline(), 'ascii') == "done":
		print("Initialization compete")
		

