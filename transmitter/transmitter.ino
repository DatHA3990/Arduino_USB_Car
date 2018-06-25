#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "config.h"

RF24 radio (CE_PIN, CSN_PIN);

byte dataIncoming[DATA_LEN_RX];
byte dataSend[DATA_LEN_TX];

unsigned long long previousTime = 0;

void setup() {
  radio.begin();
  radio.openWritingPipe(transmitterAddress);
  radio.openReadingPipe(1, receiverAddress);
  radio.setPALevel(RF24_PA_MIN);

  pinMode(LEFT_DIRECTION, OUTPUT);
  pinMode(RIGHT_DIRECTION, OUTPUT);

  pinMode(LIMIT_SWITCH, OUTPUT);
}

void loop() {
  // read joytsick values
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  // turn the joytsick values into ...
  // ... 1 byte for each axis
  dataSend[0] = byte(map(x, 0, 1023, 0, 255));
  dataSend[1] = byte(map(y, 0, 1023, 0, 255));

  // send data every 20 milliseconds
  if (millis() - previousTime >= 20) {
    radio.stopListening();
    radio.write(&dataSend, DATA_LEN_TX);
    radio.startListening();
    previousTime = millis();
  }

  // if data is received, save the values
  if (radio.available()) {
    radio.read(&dataIncoming, DATA_LEN_RX);
  }

  // write pwm values for motor ...
  // ... speed (data from rx)
  analogWrite(LEFT_MOTOR, dataIncoming[0]);
  analogWrite(RIGHT_MOTOR, dataIncoming[1]);

  // write digital values for ...
  // ... motor direction (data from rx)
  digitalWrite(LEFT_DIRECTION, dataIncoming[2]);
  digitalWrite(RIGHT_DIRECTION, dataIncoming[3]);

  // turn on or off led based on ...
  // limit switch (data from Rx)
  digitalWrite(LIMIT_SWITCH, dataIncoming[4]);
}
