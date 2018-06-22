 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio (7, 8);

const int transmitterAdress = 0xF0F0F0F0AA;
const int receiverAdress = 0xF0F0F0F0BB;


byte dataTrasmitter[2];
byte dataReceiver[5];

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, transmitterAdress);
  radio.openReadingPipe(1, receiverAdress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("test");
}

void loop() {
  
  if (radio.available(1)) {
    radio.read(&dataTrasmitter, sizeof(dataTrasmitter));

    Serial.print("From Tx : ");
    for (int i = 0; i < sizeof(dataTrasmitter); i++) {
      Serial.print(dataTrasmitter[i]);
      Serial.print(", ");
    }
    Serial.println();
  }

  if (radio.available(0)) {
    radio.read(&dataReceiver, sizeof(dataReceiver));

    Serial.print("From Rx : ");
    for (int i = 0; i < sizeof(dataReceiver); i++) {
      Serial.print(dataReceiver[i]);
      Serial.print(", ");
    }
    Serial.println();
  }
}
