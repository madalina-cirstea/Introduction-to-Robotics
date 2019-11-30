#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;

int swValue;
int xValue;
int yValue;
int startLevelValue = 0;
int currentLevel;

const int xRightTreshold = 600;
const int xLeftTreshold = 400;
const int yUpTreshold = 600;
const int yDownTreshold = 400;
const int pressingInterval = 250;
const int increasingLevelInterval = 5000;
const int playingGameInterval = 10000;
const int numberOfLives = 3;
const int mainOptionsNo = 3;
const int settingsOptionsNo = 2;

int currentMainOptionIndex = 0; //Play
int currentSettingsOptionIndex = 0; //StartLevel

int mainOptions[3][2] = {
  {1, 0}, //Play ---> lcd.setCursor(1,0)
  {7, 0}, //HiScore ---> lcd.setCursor(7,0)
  {3, 1} //Settings ---> lcd.setCursor(3,1)
};

int settingsOptions[2][2] = {
  {0, 0}, //StartLevel --->  lcd.setCursor(0, 0);
  {0, 1} // Name --->   lcd.setCursor(0, 1);
};

unsigned long lastPress = 0;

bool joyInDefaultPosition = true;
bool mainMenuLocked = false;
bool settingsMenuLocked = false;

char incomingChar;
String playerName = "________";
const int dimMaxPlayerName = 8;

bool joyMovedLeft() {
  return xValue < xLeftTreshold;
}

bool joyMovedRight() {
  return xValue > xRightTreshold;
}

bool joyMovedUp() {
  return yValue > yUpTreshold;
}

bool joyMovedDown() {
  return yValue < yDownTreshold;
}

bool joyPressed() {
  return !swValue;
}

bool joyReturnedInDefaultPosition() {
  return ((yValue >= yDownTreshold && yValue <= yUpTreshold) and (xValue >= xLeftTreshold and xValue <= xRightTreshold)) ;
}

void displayMainMenu() {
  lcd.setCursor(2, 0);
  lcd.print("Play");
  lcd.setCursor(8, 0);
  lcd.print("HiScore");
  lcd.setCursor(4, 1);
  lcd.print("Settings");
}

void displaySettingsMenu() {
  lcd.setCursor(1, 0);
  lcd.print("StartLevel:");
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
  lcd.setCursor(1, 1);
  lcd.print("Name: ");
  lcd.setCursor(7, 1);
  lcd.print(playerName);
}

void displayHiScoreMenu() {
  lcd.setCursor(0, 0);
  lcd.print("HighScore: ");
  lcd.setCursor(11, 0);
  lcd.print(EEPROM.read(0));
  lcd.setCursor(0, 1);
  lcd.print("Press JS to exit");
}

void displayPlayMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Lives: ");
  lcd.setCursor(6, 0);
  lcd.print(numberOfLives);
  lcd.setCursor(8, 0);
  lcd.print("Level: ");
  lcd.setCursor(14, 0);
  lcd.print(currentLevel);
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.setCursor(7, 1);
  lcd.print(currentLevel * 3);
}

void removeArrow(int col, int row) {
  lcd.setCursor(col, row);
  lcd.print(" ");
}

void drawArrow(int col, int row) {
  lcd.setCursor(col, row);
  lcd.print(">");
}

void moveToNextOption(int options[][2], int *currentOption, int numberOfOptions) {
  removeArrow(options[*currentOption][0], options[*currentOption][1]);
  if (*currentOption == numberOfOptions - 1)
    *currentOption = 0;
  else
    *currentOption = *currentOption + 1;
  drawArrow(options[*currentOption][0], options[*currentOption][1]);
}

void moveToPreviousOption(int options[][2], int *currentOption, int numberOfOptions) {
  removeArrow(options[*currentOption][0], options[*currentOption][1]);
  if (*currentOption == 0)
    *currentOption = numberOfOptions - 1;
  else
    *currentOption = *currentOption - 1;
  drawArrow(options[*currentOption][0], options[*currentOption][1]);
}

void updateLevelScore() {
  lcd.setCursor(14, 0);
  lcd.print(currentLevel);
  lcd.setCursor(7, 1);
  lcd.print(currentLevel * 3);
}

void goToPlayMenu() {
  currentLevel = startLevelValue;
  displayPlayMenu();

  unsigned long startGameTime = millis();
  unsigned long lastIncreasing = startGameTime;

  while (mainMenuLocked) {
    // after 5 sec increase currentLevel and update level and score
    if (millis() - lastIncreasing >= increasingLevelInterval) {
      lastIncreasing = millis();
      currentLevel++;
      updateLevelScore();
    }
    // after 10 sec game is over
    if (millis() - startGameTime >= playingGameInterval) {
      //update highScore if necessary
      if (currentLevel * 3 > EEPROM.read(0))
        EEPROM.write(0, currentLevel * 3);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game over!");
      lcd.setCursor(0, 1);
      lcd.print("Press JS to exit");

      swValue = digitalRead(pinSW);
      while (!joyPressed())
        swValue = digitalRead(pinSW);

      lastPress = millis();
      mainMenuLocked = false;
    }
  }
}

void goToHiScoreMenu() {
  displayHiScoreMenu();
  while (mainMenuLocked) {
    swValue = digitalRead(pinSW);
    if (joyPressed() and millis() - lastPress >= pressingInterval) {
      lastPress = millis();
      mainMenuLocked = false;
    }
  }
}

void goToSettingsMenu() {
  displaySettingsMenu();
  currentSettingsOptionIndex = 0; //StartLevel
  drawArrow(settingsOptions[currentSettingsOptionIndex][0], settingsOptions[currentSettingsOptionIndex][1]);
  while (mainMenuLocked) {
    settingsMenuMoves();
  }
}

void increaseStartLevel() {
  startLevelValue++;
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
}

void decreaseStartLevel() {
  if (startLevelValue > 0)
    startLevelValue--;
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
}

void modifyStartLevel() {
  while (settingsMenuLocked) {
    swValue = digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if (joyInDefaultPosition) {
      if (joyMovedUp()) {
        increaseStartLevel();
        joyInDefaultPosition = false;
      }
      else if (joyMovedDown()) {
        decreaseStartLevel();
        joyInDefaultPosition = false;
      }
      else if (joyPressed() and millis() - lastPress >= pressingInterval) {
        lastPress = millis();
        settingsMenuLocked = false;
      }
    }
    else if (joyReturnedInDefaultPosition()) {
      joyInDefaultPosition = true;
    }
  }
}

void modifyName() {
  Serial.print("Insert name. Name should contain only ");
  Serial.print(dimMaxPlayerName);
  Serial.println(" characters!");

  lcd.setCursor(7, 1);
  lcd.print("________");

  playerName = "";
  while (settingsMenuLocked) {
    if (Serial.available() > 0) {
      incomingChar = (char)Serial.read();
      if (incomingChar == '\n') {
        if (playerName.length() > dimMaxPlayerName) {
          Serial.print("Name should contain only ");
          Serial.print(dimMaxPlayerName);
          Serial.println(" characters!");
          Serial.println("Insert another name, please!");
          playerName = "";
        }
        else {
          lcd.setCursor(7, 1);
          lcd.print("         ");
          lcd.setCursor(7, 1);
          lcd.print(playerName);
          settingsMenuLocked = false;
        }
      }
      else
        playerName += incomingChar;
    }
  }
}

void settingsMenuMoves() {
  swValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (joyInDefaultPosition) {
    if (settingsMenuLocked) {
      if (currentSettingsOptionIndex == 0) {
        // settingsMenu locked on Startlevel
        modifyStartLevel();
      }
      else if (currentSettingsOptionIndex == 1) {
        // settingsMenu locked on Name
        modifyName();
      }
    }
    else {
      if (joyMovedUp() or joyMovedDown()) {
        moveToNextOption(settingsOptions, &currentSettingsOptionIndex, settingsOptionsNo);
        joyInDefaultPosition = false;
      }
      else if (joyMovedLeft()) {
        // go back to main menu
        mainMenuLocked = false;
        while (!joyReturnedInDefaultPosition()) {
          xValue = analogRead(pinX);
          yValue = analogRead(pinY);
        }
      }
      else if (joyPressed() and millis() - lastPress >= pressingInterval) {
        lastPress = millis();
        settingsMenuLocked = true;
      }
    }
  }
  else if (joyReturnedInDefaultPosition()) {
    joyInDefaultPosition = true;
  }
}

void mainMenuMoves() {
  if (joyMovedRight()) {
    moveToNextOption(mainOptions, &currentMainOptionIndex, mainOptionsNo);
    joyInDefaultPosition = false;
  }
  else if (joyMovedLeft()) {
    moveToPreviousOption(mainOptions, &currentMainOptionIndex, mainOptionsNo);
    joyInDefaultPosition = false;
  }
  else if (joyPressed() and millis() - lastPress >= pressingInterval) {
    lastPress = millis();
    mainMenuLocked = true;
    lcd.clear();
  }
}

void goBackToMainMenu() {
  lcd.clear();
  displayMainMenu();
  drawArrow(mainOptions[currentMainOptionIndex][0], mainOptions[currentMainOptionIndex][1]);
}

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  displayMainMenu();
  drawArrow(mainOptions[currentMainOptionIndex][0], mainOptions[currentMainOptionIndex][1]);
  Serial.begin(9600);
}

void loop() {
  swValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (joyInDefaultPosition) {
    mainMenuMoves();

    if (mainMenuLocked) {
      // mainMenuLocked on currentMainOptionIndex --> go to selected next menu
      if (currentMainOptionIndex == 0)
        goToPlayMenu();
      else if (currentMainOptionIndex == 1)
        goToHiScoreMenu();
      else if (currentMainOptionIndex == 2)
        goToSettingsMenu();

      goBackToMainMenu();
    }
  }
  else if (joyReturnedInDefaultPosition()) {
    joyInDefaultPosition = true;
  }
}
