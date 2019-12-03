#this file contains all the necessary functions for communications with arduino. 
#Eventually it will have functions to initialize comunication and any other required function

#use pip3 install pyserial
import serial
import time

debug = True

def sendData(ser, motN, spd):
	#ser is the serial object
	#motn is the motor number; 1-2 are for the first sabetooth, 3-4 for the second and so on...
	#spd is the speed, from 127 to -127
	speed = spd+128
	cmd = bytes("#"+str(motN)+"$"+str(speed),'utf-8')
	#time.sleep(1)
	ser.write(cmd)
	if debug: print(cmd)
	#print(str(ser.readline(), 'utf-8'))

def beginTransmission(ser):
	ser.write(bytes("begin",'utf-8'))
	if debug: print("Initialization command sent\nAwaiting response...")
	if str(ser.readline(), 'utf-8') == "done":
		print("Initialization compete")
		

