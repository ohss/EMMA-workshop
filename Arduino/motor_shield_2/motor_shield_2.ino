/**
* Get a character between R-V and trigger a solenoid
* Change values in noteDuration array to change the 'lenght' 
**/

#include <AFMotor.h>
#include <Servo.h> 

int noteDuration[] = {500, 500, 500};
unsigned long endTimes[] = {0,0,0};
unsigned long time;

// Two servos (not continuous rotation)
Servo servo1;  
Servo servo2;
int servoAngles[] = {0, 0};
Servo servos[] = {servo1, servo2};

// 3 DC motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motors[] = {motor1, motor2, motor3};

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo1.write(0);
  servo2.write(0);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
}

void loop() {
  while (Serial.available()) {
    processSerial();
  }
  
  time = millis();
  for (int i = 0; i < 3; i++) {
    if (time > endTimes[i]) {
      motors[i].run(RELEASE);
    }
  }
}

void processSerial() {
  char input = Serial.read();
  int value = charToInt(input);
  if (0 <= value && value < 2) {
    servoAngles[value] = (servoAngles[value] == 0) ? 170 : 0; 
    servos[value].write(servoAngles[value]);
  } else if (2 <= value && value < 5) {
    value = value - 2;
    motors[value].run(FORWARD);
    endTimes[value] = millis() + noteDuration[value];
  } else {
    Serial.write(input);
  }   
}

int charToInt(char c) {
  return (int)c - 82; // https://www.arduino.cc/en/Reference/ASCIIchart
}

