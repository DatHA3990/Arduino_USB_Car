import pygame, time, serial

pygame.display.init()
pygame.joystick.init()

joystick = pygame.joystick.Joystick(0).init()

null_zone = 0.15

arduino = serial.Serial('COM3', 9600)

prev = time.time()
    
while True:
    pygame.event.pump()

    direction = pygame.joystick.Joystick(0).get_axis(2)
    speed = -pygame.joystick.Joystick(0).get_axis(1)

    direction = direction * (abs(direction) > null_zone)
    speed = speed * (abs(speed) > null_zone)


    motor = [int(max(min(1, speed + (direction * [1, -1][i])), -1) * 255) for i in range(2)]

    if time.time() - prev > 0.02:
        arduino.write(bytes([abs(motor[0]),abs(motor[1]), motor[0]<0, motor[1]<1]))
        prev = time.time()


