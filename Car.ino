#define LEFT_MOTOR 3
#define RIGHT_MOTOR 11

#define LEFT_DIRECTION 12
#define RIGHT_DIRECTION 13

#define INCOMING_BYTES 4

byte values[INCOMING_BYTES];

void setup() {
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);

  pinMode(LEFT_DIRECTION, OUTPUT);
  pinMode(RIGHT_DIRECTION, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    Serial.readBytes(values, INCOMING_BYTES);
  }

  digitalWrite(LEFT_DIRECTION, values[2]);
  digitalWrite(RIGHT_DIRECTION, values[3]);

  analogWrite(LEFT_MOTOR, values[0]);
  analogWrite(RIGHT_MOTOR, values[1]);
}


