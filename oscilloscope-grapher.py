import serial
import matplotlib.pyplot as plt

ser = serial.Serial('COM3', 9600) # connect to port and set baud rate
plt.ion()
data = [] # init empty array

try:
    while True:
        if ser.in_waiting >= 2: #at least 2 bytes before graphing
            #combine into 10 bit ADC
            high_byte = ord(ser.read(1))
            low_byte = ord(ser.read(1))
            adc_value = (high_byte << 8) | low_byte

            data.append(adc_value) #stores value into array

            data = data[-100:]

            #plot:

            plt.clf()
            plt.plot(data)
            plt.pause(0.01)

except KeyboardInterrupt:
    print("plotting stopped")
    ser.close()
