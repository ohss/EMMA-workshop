int pins[] = {6,7,8,9,10,11,12,13};
unsigned long endTimes[] = {0,0,0,0,0,0,0,0};
int duration = 50;
unsigned long time = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  time = millis();
  
  while (Serial.available()) {
    processSerial();
  }
  
  for (int i = 0; i < 8; i++) {
    if (time > endTimes[i]) {
      digitalWrite(pins[i], HIGH);
    }
  }
}

void processSerial() {
  char input = Serial.read();
  int value = charToInt(input);
  if (0 <= value && value < 8) {
    digitalWrite(pins[value], LOW);
    endTimes[value] = millis() + duration;
  } else {
    Serial.write(input);
  }   
}

int charToInt(char c) {
  return (int)c - 65; // https://www.arduino.cc/en/Reference/ASCIIchart
}
