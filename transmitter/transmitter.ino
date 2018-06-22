#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define JOY_X A0
#define JOY_Y A1

#define LEFT_MOTOR 3
#define RIGHT_MOTOR 5

#define LEFT_DIRECTION 2
#define RIGHT_DIRECTION 4

#define LIMIT_SWITCH 6

byte dataIncoming[5];
byte dataSend[2];

RF24 radio (7, 8);

unsigned long long previousTime = 0;

const int sendAdress = 0xF0F0F0F0AA;
const int readAdress = 0xF0F0F0F0BB;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(sendAdress);
  radio.openReadingPipe(1, readAdress);
  radio.setPALevel(RF24_PA_MIN);

  pinMode(LEFT_DIRECTION, OUTPUT);
  pinMode(RIGHT_DIRECTION, OUTPUT);

  pinMode(LIMIT_SWITCH, OUTPUT);
}

void loop() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  dataSend[0] = byte(map(x, 0, 1023, 0, 255));
  dataSend[1] = byte(map(y, 0, 1023, 0, 255));

  if (millis() - previousTime >= 20) {
    radio.stopListening();
    radio.write(&dataSend, sizeof(dataSend));
    radio.startListening();
    previousTime = millis();
  }

  if (radio.available()) {
    radio.read(&dataIncoming, sizeof(dataIncoming));

    Serial.print("From Rx : ");
    for (int i = 0; i < sizeof(dataIncoming); i++) {
      Serial.print(dataIncoming[i]);
      Serial.print(", ");
    }
    Serial.println();
  }

  analogWrite(LEFT_MOTOR, dataIncoming[0]);
  analogWrite(RIGHT_MOTOR, dataIncoming[1]);

  digitalWrite(LEFT_DIRECTION, dataIncoming[2]);
  digitalWrite(RIGHT_DIRECTION, dataIncoming[3]);

  digitalWrite(LIMIT_SWITCH, dataIncoming[4]);
}
