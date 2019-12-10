#use pip3 install pyserial
import serial

ser = serial.Serial("/dev/ttyACM0",9600)
ser.flushInput()
while True :
	val = bytes(str(input("Type command: ")),'utf-8')
	print(val)
	ser.write(val)
	#print(ser.readline())
	print(str(ser.readline(), 'utf-8'))
