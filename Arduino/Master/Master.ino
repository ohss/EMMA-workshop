#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX

const int amountOfButtons = 5;
unsigned long time;
unsigned long nextPressTime[amountOfButtons];
unsigned long duration = 200;

char solenoid1Messages[] = {'A', 'B', 'C', 'D', 'E', 'F'};
char solenoid2Messages[] = {'G', 'H', 'I', 'J', 'K', 'L'};
char motor1Messages[] = {'M', 'N', 'O', 'P', 'Q'};
char motor2Messages[] = {'R', 'S', 'T', 'U', 'V'};

int click = 2;
int clickTime = 100;
int lastClick = 0;

int tempoPin = A0;
int tempo = 0;
int lastTempo = 0;

int solenoid1Button = 3;
int solenoid2Button = 4;
int motor1Button = 5;
int motor2Button = 6;
int resetButton = 7;

void setup() {
  pinMode(solenoid1Button, INPUT_PULLUP);
  pinMode(solenoid2Button, INPUT_PULLUP);
  pinMode(motor1Button, INPUT_PULLUP);
  pinMode(motor2Button, INPUT_PULLUP);
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
  
  if (digitalRead(motor1Button) == LOW && nextPressTime[2] < time) {
    sendCtrlMessage(motor1Messages[random(sizeof(motor1Messages))], 2);
  }
  
  if (digitalRead(motor2Button) == LOW && nextPressTime[3] < time) {
    sendCtrlMessage(motor2Messages[random(sizeof(motor2Messages))], 3);
  }
  
  if (digitalRead(resetButton) == LOW && nextPressTime[4] < time) {
    Serial.print('W');
    nextPressTime[4] = time + duration;
  }A

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
