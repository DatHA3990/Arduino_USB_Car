#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "config.h"

#define LEFT_MOTOR 3
#define RIGHT_MOTOR 5

#define LEFT_DIRECTION 2
#define RIGHT_DIRECTION 4

#define LIMIT_SWITCH 6

byte dataIncoming[dataLenTx];
byte dataSend[dataLenRx];
int values[2];

#define JOY_NULL 15

RF24 radio (7, 8);

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
  if (radio.available()) {
    radio.read(&dataIncoming, dataLenTx);

    Serial.print("From Tx : ");
    for (int i = 0; i < dataLenTx; i++) {
      Serial.print(dataIncoming[i]);
      Serial.print(", ");
    }
    Serial.println();
  }

  int x = map(dataIncoming[0], 0, 255, -255, 255);
  int y = map(dataIncoming[1], 0, 255, -255, 255);

  x *= abs(x) > JOY_NULL;
  y *= abs(y) > JOY_NULL;

  values[0] = constrain(y + x, -255, 255);
  values[1] = constrain(y - x, -255, 255);

  bool buttonVal = digitalRead(LIMIT_SWITCH);

  dataSend[0] = byte(abs(values[0]));
  dataSend[1] = byte(abs(values[1]));
  dataSend[2] = values[0] < 0;
  dataSend[3] = values[1] < 0;
  dataSend[4] = buttonVal;

  if (millis() - previousTime >= 20) {
    radio.stopListening();
    radio.write(&dataSend, dataLenRx);
    radio.startListening();
    previousTime = millis();
  }

  analogWrite(LEFT_MOTOR, dataSend[0]);
  analogWrite(RIGHT_MOTOR, dataSend[1]);

  digitalWrite(LEFT_DIRECTION, dataSend[2]);
  digitalWrite(RIGHT_DIRECTION, dataSend[3]);
}
