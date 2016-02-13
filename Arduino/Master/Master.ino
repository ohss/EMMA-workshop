#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX
const int amountOfButtons = 2;
unsigned long time;
unsigned long nextPressTime[amountOfButtons];
unsigned long duration = 200;

void setup() {
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  Serial.begin(9600);
  softSerial.begin(9600);

  for(int i = 0; i < amountOfButtons; i++) {
    nextPressTime[i] = 0;
  }
}

void loop() {
  time = millis();

  // int val1 = analogRead(A0);
  // int val2 = analogRead(A1);

  if (digitalRead(7) == LOW && nextPressTime[0] < time) {
    Serial.print('A');
    softSerial.write('A');
    nextPressTime[0] = time + duration;
  }

  if (digitalRead(8) == LOW && nextPressTime[1] < time) {
    Serial.print('I');
    softSerial.write("I");
    nextPressTime[1] = time + duration;
  }

  while (Serial.available()) {
    char value = Serial.read();
    // Serial.print(value);
    softSerial.write(value);
  }
}
