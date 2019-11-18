// joystick pins
const int pinSW = 4;
const int pinX = A0;
const int pinY = A1;

// 4 digit 7-segment display pins
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = A2;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int xRightTreshold = 600;
const int xLeftTreshold = 400;
const int yUpTreshold = 600;
const int yDownTreshold = 400;
const int pressingInterval = 250;
const int blinkInterval = 250;
const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

bool joyInDefaultPosition = true;
bool digitLocked = false;
bool waitBeforePress = false;

unsigned long lastPress = 0;
unsigned long lastBlink = 0;

int swValue = 0;
int xValue = 0;
int yValue = 0;
int number[4];
int dpState = HIGH;
int currentDisplay = 0;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int displays[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
  // a b c d e f g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1} // 9
};


void showDisplay(int displayNo) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(displays[i], HIGH);
  }
  digitalWrite(displays[displayNo], LOW);
}

void generateRandomCombination(int noOfDisplays) {
  for (int i = 0; i < noOfDisplays; i++)
    number[i] = rand() % 10;
}

void displayDigit(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void increaseDigit(int displayNo) {
  if (number[displayNo] == noOfDigits - 1)
    number[displayNo] = 0;
  else
    number[displayNo] = number[displayNo] + 1;
}

void decreaseDigit(int displayNo) {
  if (number[displayNo] == 0)
    number[displayNo] = noOfDigits - 1;
  else
    number[displayNo] = number[displayNo] - 1;
}

bool joyMovedUp() {
  return yValue > yUpTreshold;
}

bool joyMovedDown() {
  return yValue < yDownTreshold;
}

bool joyMovedLeft() {
  return xValue < xLeftTreshold;
}

bool joyMovedRight() {
  return xValue > xRightTreshold;
}

bool joyPressed() {
  return !swValue;
}

bool joyReturnedInDefaultPosition() {
  return ((yValue >= yDownTreshold && yValue <= yUpTreshold) and (xValue >= xLeftTreshold and xValue <= xRightTreshold)) ;
}

void moveToNextDisplay() {
  if (currentDisplay == noOfDisplays - 1)
    currentDisplay = 0;
  else
    currentDisplay++;
}

void moveToPreviousDisplay() {
  if (currentDisplay == 0)
    currentDisplay = noOfDisplays - 1;
  else
    currentDisplay--;
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(displays[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  generateRandomCombination(noOfDisplays);
}

void loop() {
  swValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  for (int displayNo = 0; displayNo < noOfDisplays; displayNo++) {
    showDisplay(displayNo);
    if (displayNo == currentDisplay)
      displayDigit(number[displayNo], dpState);
    else
      displayDigit(number[displayNo], LOW);
    delay(5);
  }

  if (joyInDefaultPosition) {
    if (joyPressed() && waitBeforePress) {
      if (millis() - lastPress >= pressingInterval)
        waitBeforePress = false;
    }
    if (digitLocked) {
      // digit on current display is locked
      if (joyMovedUp()) {
        // increase digit on current display
        increaseDigit(currentDisplay);
        joyInDefaultPosition = false;
      }
      else if (joyMovedDown()) {
        // decrease digit on current display
        decreaseDigit(currentDisplay);
        joyInDefaultPosition = false;
      }
      else if (joyPressed() && !waitBeforePress) {
        // unlock selected display
        digitLocked = false;
        lastPress = millis();
        waitBeforePress = true;
      }
    }
    else {
      // digit on current display is unlocked
      if (millis() - lastBlink >= blinkInterval) {
        dpState = !dpState;
        lastBlink = millis();
      }
      if (joyMovedLeft()) {
        // joy moved left => move to next display
        moveToNextDisplay();
        joyInDefaultPosition = false;
      }
      else if (joyMovedRight()) {
        // joy moved right => move to previous display
        moveToPreviousDisplay();
        joyInDefaultPosition = false;
      }
      else if (joyPressed() && !waitBeforePress) {
        // lock on selected display
        digitLocked = true;
        lastPress = millis();
        waitBeforePress = true;
        dpState = HIGH;
      }
    }
  }
  else if (joyReturnedInDefaultPosition()) {
    joyInDefaultPosition = true;
  }
}
