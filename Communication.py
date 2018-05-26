# Class to communicate from PC to Arduino via USB serial communication
# by Dat HA on May 25th 2018

import serial # to communicate via serial to the Arduino
import time   # to send the data at a specific frequency

class Arduino: # start class
    def __init__(self, com_port): # class constructor, com_port is the serial communication port that is used to talk to the Arduino
        self.arduino = serial.Serial(com_port, 9600) # initialize serial communication with a baud rate of 9600
        self.prev = time.time() # set "previous" time to now
        self.data = "" # data variable

    def update(self, data): # update function, to be called all the time in script, data is a parameter that contains user data
        self.data = data # set class data parameter to local parameter because the data might not be sent this time yet
        if time.time() - self.prev > 0.05: # if the difference between the current time and the previous time is greater then a certain value, send the data
            self.arduino.write(str.encode(self.data)) # write the data to the serial port
            self.prev = time.time() # set the new previous time
