const int speakerPin = A0;
const int activeBuzzerPin = 11;
const int buttonPin = 2;
const int threshold = 0;

int speakerValue = 0;
int buttonState = 0;
int buzzerTone = 1000;
int waitBeforeKnocking = 0;

unsigned long previousMillis = 0;
const long interval = 500;

bool knocked = false;
bool soundOn = false;

void setup() {
  pinMode(speakerPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (!waitBeforeKnocking) {
    if (knocked == false) {
      speakerValue = analogRead(speakerPin);
      if (speakerValue > threshold) {
        knockButton();
        previousMillis = millis();
      }
    }
    else {
      if (millis() - previousMillis >= interval && soundOn == false)
        playSound();
      buttonState = digitalRead(buttonPin);
      if (soundOn == true && buttonState == HIGH) {
        stopSound();
        knocked = false;
        previousMillis = millis();
        waitBeforeKnocking = 1;
      }
    }
  }
  else {
    if (millis() - previousMillis >= interval)
      waitBeforeKnocking = 0;
  }
}

void knockButton() {
  Serial.println("Knocked");
  knocked = true;
}

void stopSound() {
  Serial.println("Sound OFF");
  soundOn = false;
  noTone(activeBuzzerPin);
}

void playSound() {
  Serial.println("Sound ON");
  soundOn = true;
  tone(activeBuzzerPin, buzzerTone);
}
