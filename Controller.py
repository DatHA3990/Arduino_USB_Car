# Controller for Arduino car
# by Dat HA on May 25th 2018

import pygame               # import pygame library for joystick controls
from Communication import * # import our Arduino-PC communication class

pygame.display.init()  # start pygame display (no actual use, just necessary)
pygame.joystick.init() # start a joystick

joystick = pygame.joystick.Joystick(0).init() # initiate joystick

null_zone = 0.15 # set zone at which joystick will not react

arduino = Arduino('COM3') # start communication with Arduino

    
while True: # infinite loop until the communication is severed
    pygame.event.pump() # update joystick

    direction = pygame.joystick.Joystick(0).get_axis(2) # get joystick direction value
    speed = -pygame.joystick.Joystick(0).get_axis(1)    # get joystick speed value

    direction = direction * (abs(direction) > null_zone) # if direction value is in null zone, set it to 0
    speed = speed * (abs(speed) > null_zone)             # if speed value is in null zone, set it to 0


    motor = [int(max(min(1, speed + (direction * [1, -1][i])), -1) * 255) for i in range(2)] # translate direction and speed value to motor value for tank drive

    arduino.update(str(motor[0])+','+str(motor[1])) # send if to the Arduino
