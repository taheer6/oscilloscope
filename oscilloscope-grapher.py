import serial
import matplotlib.pyplot as plt

ser = serial.Serial('COM3', 9600) # connect to port and set baud rate
plt.ion()
data = [] # init empty array

