#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio (7, 8);

const int transmitterAdress = 0xF0F0F0F0AA;
const int receiverAdress = 0xF0F0F0F0BB;

uint8_t pipeTx = 1;
uint8_t pipeRx = 2;

void setup() {
  Serial.begin(9600);

  radio.begin();

  radio.openReadingPipe(pipeTx, transmitterAdress);
  radio.openReadingPipe(pipeRx, receiverAdress);
  radio.startListening();

  radio.setPALevel(RF24_PA_MIN);
  Serial.println("test");
}

void loop() {
  byte pipe;

  byte data[32] = {NULL};
  if (radio.available(&pipe)) {
    radio.read(data, 32);
    Serial.print(pipe);
    Serial.print("  :  ");

    if (pipe == pipeTx) {
      Serial.print("tx : ");
      for (int i = 0; i < 2; i++) {
        Serial.print(data[i]);
        Serial.print(", ");
      }
    }

    if (pipe == pipeRx) {
      Serial.print("rx : ");
      for (int i = 0; i < 5; i++) {
        Serial.print(data[i]);
        Serial.print(", ");
      }
    }
  }
  Serial.println();
}
