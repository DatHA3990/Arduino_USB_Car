#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio (7, 8);

const int transmitterAdress = 0xF0F0F0F0AA;
const int receiverAdress = 0xF0F0F0F0BB;

byte pipeTx = 1;
byte pipeRx = 2;

byte dataTrasmitter[2];
byte dataReceiver[5];

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
  if (radio.available(&pipeTx)) {
    radio.read(&dataTrasmitter, sizeof(dataTrasmitter));

    Serial.print("From Tx : ");
    for (int i = 0; i < sizeof(dataTrasmitter); i++) {
      Serial.print(dataTrasmitter[i]);
      Serial.print(", ");
    }
    Serial.println();
  }


  if (radio.available(&pipeRx)) {
    radio.read(&dataReceiver, sizeof(dataReceiver));

    Serial.print("From Rx : ");
    for (int i = 0; i < sizeof(dataReceiver); i++) {
      Serial.print(dataReceiver[i]);
      Serial.print(", ");
    }
    Serial.println();
  }
}
