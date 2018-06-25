#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "config.h"

RF24 radio (CE_PIN, CSN_PIN);

byte dataIncoming[DATA_LEN_TX];
byte dataSend[DATA_LEN_RX];
int values[2];

unsigned long long previousTime = 0;

void setup() {
  pinMode(LEFT_DIRECTION, OUTPUT);
  pinMode(RIGHT_DIRECTION, OUTPUT);

  pinMode(LIMIT_SWITCH, INPUT);

  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(1, transmitterAddress);
  radio.openWritingPipe(receiverAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // if data is received, save the values
  if (radio.available()) {
    radio.read(&dataIncoming, DATA_LEN_TX);
  }

  // convert 1 byte per axis into 2 bytes
  int x = map(dataIncoming[0], 0, 255, -255, 255);
  int y = map(dataIncoming[1], 0, 255, -255, 255);

  // make sure that value is above ..
  // ... the joysticks null zones
  x *= abs(x) > JOY_NULL;
  y *= abs(y) > JOY_NULL;

  // add the "turn" to the "speed" and cap ...
  // ... their values between -255 and 255
  values[0] = constrain(y + x, -255, 255);
  values[1] = constrain(y - x, -255, 255);

  // read limit switch
  bool buttonVal = digitalRead(LIMIT_SWITCH);

  // prepare all values
  dataSend[0] = byte(abs(values[0]));
  dataSend[1] = byte(abs(values[1]));
  dataSend[2] = values[0] < 0;
  dataSend[3] = values[1] < 0;
  dataSend[4] = buttonVal;

  // send data every 20 milliseconds
  if (millis() - previousTime >= 20) {
    radio.stopListening();
    radio.write(&dataSend, DATA_LEN_RX);
    radio.startListening();
    previousTime = millis();
  }

  // write pwm formotor speed
  analogWrite(LEFT_MOTOR, dataSend[0]);
  analogWrite(RIGHT_MOTOR, dataSend[1]);

  // write digital for motor direction
  digitalWrite(LEFT_DIRECTION, dataSend[2]);
  digitalWrite(RIGHT_DIRECTION, dataSend[3]);
}
