// Code for a "remote" control car from a PC to the Arduino vehicle via USB serial communication
// by Dat HA on May 25th 2018

#define LEFT_MOTOR 3   // left motor port
#define RIGHT_MOTOR 11 // right motor port

#define LEFT_DIRECTION 12  // left motor direction pin port
#define RIGHT_DIRECTION 13 // right motor direction pin port

int left = 0;  // left motor value, range from 0 to 255
int right = 0; // right motor value, range from 0 to 255

void setup() {
  pinMode(LEFT_MOTOR, OUTPUT);  // set left motor port as output
  pinMode(RIGHT_MOTOR, OUTPUT); // set right motor port as output

  pinMode(LEFT_DIRECTION, OUTPUT);  // set left motor direction pin port as output
  pinMode(RIGHT_DIRECTION, OUTPUT); // set right motor direction pin port as output

  Serial.begin(9600);   // start serial communication
  Serial.setTimeout(3); // set serial timeout to 3 milliseconds, prevent code from "freezing"
}

void loop() {
  if (Serial.available()) { // is a message has been transfered from the PC
    String data = Serial.readString(); // read the message and store it as a string
    
    for (int i = 0; i < data.length(); i++) { // iterate throught the message
      if (data.substring(i, i + 1) == ",") { // if the char that we are on is the divider char...
        left = data.substring(0, i).toInt(); // ... set the left motor value as the one before the divider char...
        right = data.substring(i + 1).toInt(); // ... and the right motor value as the one after the divider char
        break; // exit for loop
      }
    }

    digitalWrite(LEFT_DIRECTION, left<0);   // set the left motor direction pin to HIGH if the speed of the motor is negative, else keep it LOW
    digitalWrite(RIGHT_DIRECTION, right<0); // set the right motor direction pin to HIGH if the speed of the motor is negative, else keep it LOW

    analogWrite(LEFT_MOTOR, abs(left));   // set the left motor value to the absolute value of the left motor
    analogWrite(RIGHT_MOTOR, abs(right)); // set the right motor value to the absolute value of the right motor
  }
}

