#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX

const int amountOfButtons = 5;
unsigned long time;
unsigned long nextPressTime[amountOfButtons];
unsigned long duration = 200;

char solenoid2Messages[] = {'A', 'B', 'C', 'E', 'F'};
char solenoid1Messages[] = {'G', 'H', 'I', 'J', 'K', 'L'};
char dc1Messages[] = {'O', 'P', 'Q'};
char dc2Messages[] = {'T', 'U', 'V'};
char servo1Messages[] = {'M', 'N'};
char servo2Messages[] = {'R', 'S'};

int click = 5;
int clickTime = 100;
int lastClick = 0;

int tempoPin = A2;
int tempo = 0;
int lastTempo = 0;

int solenoid1Button = 2;
int solenoid2Button = 3;
int dcPin = A1;
int servoPin = A0;
int resetButton = 4;

void setup() {
  pinMode(solenoid1Button, INPUT_PULLUP);
  pinMode(solenoid2Button, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(click, OUTPUT);
  
  Serial.begin(9600);
  softSerial.begin(9600);

  for(int i = 0; i < amountOfButtons; i++) {
    nextPressTime[i] = 0;
  }
}

void loop() {
  time = millis();
  
  if (digitalRead(solenoid1Button) == LOW && nextPressTime[0] < time) {
    sendCtrlMessage(solenoid1Messages[random(sizeof(solenoid1Messages))], 0);
  }
  
  if (digitalRead(solenoid2Button) == LOW && nextPressTime[1] < time) {
    sendCtrlMessage(solenoid2Messages[random(sizeof(solenoid2Messages))], 1);
  }
  
  if (nextPressTime[2] < time) {
    int dcValue = analogRead(dcPin);
    if (dcValue < 10) {
      sendCtrlMessage(dc1Messages[random(sizeof(dc1Messages))], 2);
    } else if (dcValue > 1013) {
      sendCtrlMessage(dc2Messages[random(sizeof(dc2Messages))], 2);
    }
  }
  
   if (nextPressTime[3] < time) {
    int servoValue = analogRead(servoPin);
    if (servoValue < 10) {
      sendCtrlMessage(servo1Messages[random(sizeof(servo1Messages))], 3);
    } else if (servoValue > 1013) {
      sendCtrlMessage(servo2Messages[random(sizeof(servo2Messages))], 3);
    }
  }
  
  if (digitalRead(resetButton) == LOW && nextPressTime[4] < time) {
    Serial.print('W');
    nextPressTime[4] = time + duration;
  }

  while (Serial.available()) {
    char value = Serial.read();
    if (value == 'X') {
      digitalWrite(click, HIGH);
      lastClick = time;
    }
    softSerial.write(value);
  }
  
  if (time - lastClick > clickTime) {
    digitalWrite(click, LOW);
  }
  
  tempo = (int) map(analogRead(tempoPin), 0, 1023, 0, 9);
  if (tempo != lastTempo) {
    Serial.print(tempo);
    lastTempo = tempo;
  }  
}

void sendCtrlMessage(char x, int buttonIndex) {
  Serial.print(x);
  softSerial.write(x);
  nextPressTime[buttonIndex] = time + duration;
}
