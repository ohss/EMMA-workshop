#include <AFMotor.h>
#include <Servo.h> 
 
Servo servo1;  
Servo servo2;  

int pos1;
int pos2;

static char servo1Id = 'Q';
static char servo2Id = 'R';
unsigned long time;
unsigned long endTime1;
unsigned long endTime2;
const long duration1 = 250;
const long duration2 = 250;
const int servo1Rest = 90;
const int servo2Rest = 90;

void setup() {
  servo1.attach(9);
  servo2.attach(10);
  Serial.begin(9600);
  endTime1 = millis();
  endTime2 = millis();
}

void loop() {

  time = millis();
  while (Serial.available()) {
    processSerial();
  }

  if (time > endTime1) {
    servo1.write(servo1Rest); 
  }

  if (time > endTime2) {
    servo2.write(servo2Rest); 
  }  

  /*
  delay(1000);
  servo2.write(140);
  delay(1000);
  servo1.write(40);
  delay(1000);
  */
}

void processSerial() {

  char value = Serial.read();
  if (value == servo1Id) {
    servo1.write(0); 
    endTime1 = millis() + duration1;
  } else if (value == servo2Id) {
    servo2.write(0);
    endTime2 = millis() + duration2;
  } else {
    Serial.write(value);
    Serial.print(value);
  }
}

