#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "config.h"

RF24 radio (7, 8);

uint8_t pipeTx = 1;
uint8_t pipeRx = 2;

void setup() {
  Serial.begin(9600);

  radio.begin();

  radio.openReadingPipe(pipeTx, transmitterAddress);
  radio.openReadingPipe(pipeRx, receiverAddress);
  radio.startListening();

  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  byte pipe;

  // check if the radio is available
  // this func will set the var pipe to the ...
  // ... number of the pipe that is available
  if (radio.available(&pipe)) {
    
    if (pipe == pipeTx) {
      byte data[dataLenTx];
      radio.read(data, dataLenTx);
      Serial.print("tx : ");
      for (int i = 0; i < dataLenTx; i++) {
        Serial.print(data[i]);
        Serial.print(", ");
      }
      Serial.println();
    }

    else if (pipe == pipeRx) {
      byte data[dataLenRx];
      radio.read(data, dataLenRx);
      Serial.print("rx : ");
      for (int i = 0; i < dataLenRx; i++) {
        Serial.print(data[i]);
        Serial.print(", ");
      }
      Serial.println();
    }
  }
}
