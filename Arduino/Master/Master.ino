#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX
const int amountOfButtons = 2;
unsigned long time;
unsigned long nextPressTime[amountOfButtons];
unsigned long duration = 200;

char solenoidMessages[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
char motorMessages[] = {'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};

int click = 2;
int clickTime = 100;
int lastClick = 0;

int tempoPin = A0;
int tempo = 0;
int lastTempo = 0;

void setup() {
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(click, OUTPUT);
  
  Serial.begin(9600);
  softSerial.begin(9600);

  for(int i = 0; i < amountOfButtons; i++) {
    nextPressTime[i] = 0;
  }
}

void loop() {
  time = millis();

  if (digitalRead(7) == LOW && nextPressTime[0] < time) {
    sendSolenoidMessage();
  }

  if (digitalRead(8) == LOW && nextPressTime[1] < time) {
    sendMotorMessage();
  }
  
  if (time - lastClick > clickTime) {
    digitalWrite(click, LOW);
  }

  while (Serial.available()) {
    char value = Serial.read();
    if (value == 'X') {
      digitalWrite(click, HIGH);
      lastClick = time;
    }
    softSerial.write(value);
  }
  
  tempo = (int) map(analogRead(tempoPin), 0, 1023, 0, 9);
  if (tempo != lastTempo) {
    Serial.print(tempo);
    lastTempo = tempo;
  }  
}

void sendSolenoidMessage() {
  char x = solenoidMessages[random(sizeof(solenoidMessages))];
  Serial.print(x);
  softSerial.write(x);
  nextPressTime[0] = time + duration;
}

void sendMotorMessage() {
  char y = motorMessages[random(sizeof(motorMessages))];
  Serial.print(y);
  softSerial.write(y);
  nextPressTime[1] = time + duration;
}
