#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "config.h"

RF24 radio (CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(9600);

  radio.begin();

  radio.openReadingPipe(PIPE_TX, transmitterAddress);
  radio.openReadingPipe(PIPE_RX, receiverAddress);
  radio.startListening();

  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  byte pipe;

  // check if the radio is available
  // this func will set the var pipe to the ...
  // ... number of the pipe that is available
  if (radio.available(&pipe)) {

    // switch case depending on the pipe
    switch (pipe) {

      // transmitter's pipe
      case PIPE_TX : {
          byte data[DATA_LEN_TX];
          radio.read(data, DATA_LEN_TX);
          Serial.print("tx : ");
          for (int i = 0; i < DATA_LEN_TX; i++) {
            Serial.print(data[i]);
            Serial.print(", ");
          }
          Serial.println();
          break;
        }

      // receiver's pipe
      case PIPE_RX: {
          byte data[DATA_LEN_RX];
          radio.read(data, DATA_LEN_RX);
          Serial.print("rx : ");
          for (int i = 0; i < DATA_LEN_RX; i++) {
            Serial.print(data[i]);
            Serial.print(", ");
          }
          Serial.println();
          break;
        }
    }
  }
}
