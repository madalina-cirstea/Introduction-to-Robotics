#include "LedControl.h"
#include "joystick.h"
#include <stdlib.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>
#include "pitches.h"

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define STRAIGHT 0
#define MIN_TIME_BEFORE_MOVE 150
#define MAX_TIME_BEFORE_MOVE 375

const int buzzerPin = 6;

const int pinX = A0;
const int pinY = A1;
const int pinSW = A2;

const int RS = 8;
const int enable = 9;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(12, 11, 10, 1);

const int noOfLevels = 4;
const int noOfLives = 3;
const int mainOptionsNo = 4;
const int settingsOptionsNo = 2;
const int noOfBricks[4] = {12, 21, 20, 24};

int rowDirection;
int colDirection;
int startLevelValue = 1;
int currentLevel;
int lives;
int destroyedBricks;
int score;
int currentMainOptionIndex = 0; //Play
int currentSettingsOptionIndex = 0; //StartLevel

const int mainOptions[4][2] = {
  {1, 0}, //Play ---> lcd.setCursor(1,0)
  {7, 0}, //HiScore ---> lcd.setCursor(7,0)
  {0, 1}, //Settings ---> lcd.setCursor(0,1)
  {10, 1} //Info  ---> lcd.setCursor(10,1)
};

const int settingsOptions[2][2] = {
  {0, 0}, //StartLevel --->  lcd.setCursor(0, 0);
  {0, 1} // Name --->   lcd.setCursor(0, 1);
};

bool levelFrame[8][8];

const bool levels[4][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0}
  },
  {
    {1, 1, 1, 0, 0, 0, 1, 0},
    {1, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 1, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {1, 1, 1, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0}
  },
  {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0}
  },
  {
    {0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0}
  }
};

const bool levelNumber[4][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0}
  }
};

bool mainMenuLocked = false;
bool settingsMenuLocked = false;
bool letterLocked;
bool canShoot = true;
bool gameOver;
bool restore;

char playerName[7] = "ANONIM";

struct Ball {
  int row = 6, col = 3;
} ball;

struct Paddle {
  int left = 2, center = 3, right = 4;
} paddle;

struct Flame {
  int row, col;
  bool destroyed = true;
} flameLeft, flameRight;


unsigned int waitBeforeBallMove;
const unsigned int decreaseTime = 75;
unsigned long lastBallMove = 0;

const unsigned int waitBeforePaddleMove = 75;
unsigned long lastPaddleMove = 0;

const unsigned int waitBeforeBallBlink = 20;
unsigned long lastBallBlink = 0;

const unsigned int waitBeforeFlameMove = 100;
unsigned long lastFlameMove = 0;

const unsigned long waitForLoadingLevelNumber = 3000;

unsigned long restoreTime;

Joystick joy;

byte heart[] = {
  B00000,
  B00000,
  B11011,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte arrowUp1[] = {
  B00000,
  B00000,
  B00100,
  B01010,
  B10001,
  B00100,
  B00100,
  B00100
};

byte arrowUp2[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000
};

byte arrowDown1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00100,
  B00100
};

byte arrowDown2[] = {
  B00100,
  B00100,
  B00100,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000
};

byte arrowUp[] = {
  B00100,
  B01010,
  B10001,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

int santa_melody[] = {
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int santa_tempo[] = {
  4, 4, 4, 4,
  4, 2, 4,
  1
};

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(targetPin, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  // 1 second's worth of microseconds, divided by the frequency, then split in half since there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  // multiply frequency, which is really cycles per second, by the number of seconds to get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again for the calculated delay value
  }
  digitalWrite(targetPin, LOW);
}

void song() {
  int size = sizeof(santa_melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8
    int noteDuration = 900 / santa_tempo[thisNote];
    buzz(buzzerPin, santa_melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(buzzerPin, 0, noteDuration);
  }
}

void displayMainMenu() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Play");
  lcd.setCursor(8, 0);
  lcd.print("HiScore");
  lcd.setCursor(1, 1);
  lcd.print("Settings");
  lcd.setCursor(11, 1);
  lcd.print("Info");
}

void displayInfoScreen1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.setCursor(2, 0);
  lcd.print("Brick Braker");
  lcd.setCursor(1, 1);
  lcd.print("@UniBucRobotics");
}

void displayInfoScreen2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(4);
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.setCursor(2, 0);
  lcd.print("Game: MadaC");
  lcd.setCursor(2, 1);
  lcd.print("Case: AlexT");
}

void displayInfoScreen3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(6);
  lcd.setCursor(2, 0);
  lcd.print("github:");
  lcd.setCursor(0, 1);
  lcd.print("madalina-cirstea");
}

void displaySettingsMenu() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("StartLevel: ");
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
  lcd.setCursor(1, 1);
  lcd.print("Name: ");
  lcd.setCursor(7, 1);
  lcd.print(playerName);
}

void displayHiScoreMenu() {
  lcd.clear();
  for (int i = 2; i <= 7; i++) {
    lcd.setCursor(i - 2, 0);
    lcd.print(char(EEPROM.read(i)));
  }
  for (int i = 8; i <= 13; i++) {
    lcd.setCursor(i - 8, 1);
    lcd.print(char(EEPROM.read(i)));
  }
  lcd.setCursor(8, 0);
  lcd.print(EEPROM.read(0));
  lcd.setCursor(8, 1);
  lcd.print(EEPROM.read(1));
}

void displayPlayMenu() {
  lcd.clear();
  for (int i = 0; i < lives; i++) {
    lcd.setCursor(i, 0);
    lcd.write(1);
  }
  lcd.setCursor(8, 0);
  lcd.print("Level: ");
  lcd.setCursor(14, 0);
  lcd.print(currentLevel);
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.setCursor(7, 1);
  lcd.print(score);
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

void updateLives() {
  lcd.setCursor(lives, 0);
  lcd.print(" ");
}

void updateScore() {
  lcd.setCursor(7, 1);
  lcd.print("   ");
  lcd.setCursor(7, 1);
  lcd.print(score);
}

void initializeLevel() {
  if (!flameLeft.destroyed)
    destroyFlameLeft();
  if (!flameRight.destroyed)
    destroyFlameRight();

  waitBeforeBallMove = MAX_TIME_BEFORE_MOVE;
  lives = 3;
  destroyedBricks = 0;
  ball.row = 6;
  ball.col = 3;
  paddle.left = 2;
  paddle.center = 3;
  paddle.right = 4;

  createLevelFrame();
}

void restoreLevel() {
  if (!flameLeft.destroyed)
    destroyFlameLeft();
  if (!flameRight.destroyed)
    destroyFlameRight();
  restoreBallPaddle();
  rowDirection = UP;
  colDirection = randomColDirection();
  restore = false;
}

void loseLife() {
  Serial.println("lose life");
  lives--;
  updateLives();
  restore = true; // level must be restored
  restoreTime = millis();
}

void shoot()  {
  tone(buzzerPin, NOTE_C4, 100);
  score -= 5;
  updateScore();
  initializeFlames();
  canShoot = false;
}

int playLevel() {
  while (noOfBricks[currentLevel - 1] != destroyedBricks) {
    if (millis() - restoreTime >= 1500) {
      if (lives == 0) {
        gameOver = true;
        return -1;
      }

      if (restore == true)
        restoreLevel();

      if (millis() - lastBallBlink >= waitBeforeBallBlink) {
        levelFrame[ball.row][ball.col] = !levelFrame[ball.row][ball.col];
        lc.setLed(0, ball.row, ball.col, levelFrame[ball.row][ball.col]);
        lastBallBlink = millis();
      }

      if (millis() - lastPaddleMove >= waitBeforePaddleMove) {
        joy.setValueSW(digitalRead(pinSW));
        joy.setValueX(analogRead(pinX));
        joy.setValueY(analogRead(pinY));

        if (joy.movedLeft())
          movePaddleLeft();
        else if (joy.movedRight())
          movePaddleRight();

        lastPaddleMove = millis();
      }

      joy.setValueSW(digitalRead(pinSW));
      if (joy.pressed() && canShoot && score >= 5) {
        shoot();
        lastFlameMove = millis();
      }

      if ((!flameLeft.destroyed || !flameRight.destroyed) && millis() - lastFlameMove >= waitBeforeFlameMove) {
        moveFlames();
        lastFlameMove = millis();
      }

      if (flameLeft.destroyed && flameRight.destroyed)
        canShoot = true;

      if (millis() - lastBallMove >= waitBeforeBallMove && !restore) {
        moveBall();
        lastBallMove = millis();
      }
    }
  }

  //level complete => move to next level
  currentLevel++;
  if (currentLevel > noOfLevels)
    gameOver = true;

  return 0;
}

void loadCurrentLevelNumber() {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      lc.setLed(0, row, col, levelNumber[currentLevel - 1][row][col]);
}

void goToPlayMenu() {
  currentLevel = startLevelValue;
  gameOver = false;
  score = 0;
  initializeLevel();
  displayPlayMenu();
  unsigned long loadLevelNumberTime = millis();

  while (!gameOver) {
    loadCurrentLevelNumber();
    if (millis() - loadLevelNumberTime >= waitForLoadingLevelNumber) {
      clearMatrix();
      loadLevelFrame();
      playLevel();
      delay(2000); // wait before loading next level
      clearMatrix();
      loadLevelNumberTime = millis();
      initializeLevel();
      displayPlayMenu();
    }
  }

  //update highScore if necessary
  int playerPlace = 0; // player score is smaller than first 2 highscores
  if (score > EEPROM.read(0)) {
    EEPROM.write(1,  EEPROM.read(0));
    EEPROM.write(0, score);
    for (int i = 2; i <= 7; i++) {
      EEPROM.write(i + 6, EEPROM.read(i));
      EEPROM.write(i, playerName[i - 2]);
    }
    playerPlace = 1;
  }
  else if (score >  EEPROM.read(1)) {
    EEPROM.write(1, score);
    for (int i = 8; i <= 13; i++)
      EEPROM.write(i, playerName[i - 8]);
    playerPlace = 2;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over!");

  if (playerPlace == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.setCursor(7, 1);
    lcd.print(score);
  }
  else if (playerPlace == 1) {
    lcd.setCursor(0, 1);
    lcd.print("1st place ");
    lcd.setCursor(11, 1);
    lcd.print(score);
  }
  else if (playerPlace == 2) {
    lcd.setCursor(0, 1);
    lcd.print("2nd place  ");
    lcd.setCursor(11, 1);
    lcd.print(score);
  }

  joy.setValueSW(digitalRead(pinSW));
  while (!joy.pressed()) {
    joy.setValueSW(digitalRead(pinSW));
  }
  joy.setLastPress(millis());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PlayAgain? press");
  lcd.setCursor(0, 1);
  lcd.print(" < MENU");

  while (mainMenuLocked) {
    joy.setValueSW(digitalRead(pinSW));
    joy.setValueX(analogRead(pinX));

    if (joy.movedLeft()) {
      mainMenuLocked = false;
      while (!joy.returnedInDefaultPosition()) {
        joy.setValueX(analogRead(pinX));
        joy.setValueY(analogRead(pinY));
      }
    }
    else if (joy.pressed() and millis() - joy.getLastPress()  >= joy.getPressingInterval()) {
      joy.setLastPress(millis());
      goToPlayMenu();
    }
  }
}

void goToHiScoreMenu() {
  displayHiScoreMenu();
  while (mainMenuLocked) {
    joy.setValueSW(digitalRead(pinSW));
    if (joy.pressed() and millis() - joy.getLastPress() >= joy.getPressingInterval()) {
      joy.setLastPress(millis());
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

void displayInfoScreen(int currentScreen) {
  if (currentScreen == 1)
    displayInfoScreen1();
  else if (currentScreen == 2)
    displayInfoScreen2();
  else displayInfoScreen3();
}

void goToInfoMenu() {
  int currentScreen = 1;
  displayInfoScreen(currentScreen);

  while (mainMenuLocked) {
    joy.setValueSW(digitalRead(pinSW));
    joy.setValueY(analogRead(pinY));

    if (joy.getDefaultPosition()) {
      if (joy.movedUp()) {
        if (currentScreen > 1) {
          currentScreen--;
          displayInfoScreen(currentScreen);
        }
        joy.setDefaultPosition(false);
      }
      else if (joy.movedDown()) {
        if (currentScreen < 3) {
          currentScreen++;
          displayInfoScreen(currentScreen);
        }
        joy.setDefaultPosition(false);
      }
      else if (joy.pressed() and millis() - joy.getLastPress()  >= joy.getPressingInterval()) {
        joy.setLastPress(millis());
        mainMenuLocked = false;
      }
    }
    else if (joy.returnedInDefaultPosition()) {
      joy.setDefaultPosition(true);
    }
  }
}

void increaseStartLevel() {
  startLevelValue++;
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
}

void decreaseStartLevel() {
  startLevelValue--;
  lcd.setCursor(13, 0);
  lcd.print(startLevelValue);
}

void modifyStartLevel() {
  while (settingsMenuLocked) {
    joy.setValueSW(digitalRead(pinSW));
    joy.setValueX(analogRead(pinX));
    joy.setValueY(analogRead(pinY));

    if (joy.getDefaultPosition()) {
      if (joy.movedUp()) {
        if (startLevelValue < noOfLevels) {
          increaseStartLevel();
          clearMatrix();
          loadLevel(startLevelValue - 1);
        }
        joy.setDefaultPosition(false);
      }
      else if (joy.movedDown()) {
        if (startLevelValue > 1) {
          decreaseStartLevel();
          clearMatrix();
          loadLevel(startLevelValue - 1);
        }
        joy.setDefaultPosition(false);
      }
      else if (joy.pressed() and millis() - joy.getLastPress()  >= joy.getPressingInterval()) {
        joy.setLastPress(millis());
        settingsMenuLocked = false;
        clearMatrix();
      }
    }
    else if (joy.returnedInDefaultPosition()) {
      joy.setDefaultPosition(true);
    }
  }
}

void updateLetter(char letter, int letterIndex) {
  lcd.setCursor(letterIndex, 1);
  lcd.print(letter);
}

void modifyLetter(int letterIndex) {
  char letter = '_';
  updateLetter(letter, letterIndex);

  while (letterLocked) {
    joy.setValueSW(digitalRead(pinSW));
    joy.setValueX(analogRead(pinX));
    joy.setValueY(analogRead(pinY));

    if (joy.getDefaultPosition()) {
      if (joy.movedUp()) {
        joy.setDefaultPosition(false);
        if (letter == 'Z' || letter == '_')
          letter = 'A';
        else if (letter < 'Z')
          letter = char(letter + 1);
        updateLetter(letter, letterIndex);
        playerName[letterIndex % 7] = letter;
      }
      else if (joy.movedDown()) {
        joy.setDefaultPosition(false);
        if (letter == 'A' || letter == '_')
          letter = 'Z';
        else  if (letter > 'A')
          letter = char(letter - 1);
        updateLetter(letter, letterIndex);
        playerName[letterIndex % 7] = letter;
      }
      else if (joy.pressed() and millis() - joy.getLastPress() >= joy.getPressingInterval()) {
        joy.setLastPress(millis());
        letterLocked = false;
      }
    }
    else if (joy.returnedInDefaultPosition()) {
      joy.setDefaultPosition(true);
    }
  }
}

void blinkUnderline(int i, int letterIndex) {
  if (i % 2) {
    lcd.setCursor(letterIndex, 1);
    lcd.print(playerName[letterIndex % 7]);
  }
  else {
    lcd.setCursor(letterIndex, 1);
    lcd.print("_");
  }
}

void modifyName() {
  int letterIndex = 7, i = 0;
  unsigned long lastChange = millis();
  letterLocked = false;

  while (settingsMenuLocked) {
    joy.setValueSW(digitalRead(pinSW));
    joy.setValueX(analogRead(pinX));
    joy.setValueY(analogRead(pinY));

    if (millis() - lastChange >= 400) {
      blinkUnderline(i, letterIndex);
      i++;
      lastChange = millis();
    }

    if (joy.getDefaultPosition()) {
      if (joy.movedRight()) {
        lcd.setCursor(letterIndex, 1);
        lcd.print(playerName[letterIndex % 7]);

        if (letterIndex < 12)
          letterIndex++;
        else
          letterIndex = 7;

        joy.setDefaultPosition(false);
      }
      else if (joy.movedLeft()) {
        lcd.setCursor(letterIndex, 1);
        lcd.print(playerName[letterIndex % 7]);

        settingsMenuLocked = false;

        while (!joy.returnedInDefaultPosition()) {
          joy.setValueX(analogRead(pinX));
          joy.setValueY(analogRead(pinY));
        }
      }
      else if (joy.pressed() and millis() - joy.getLastPress() >= joy.getPressingInterval()) {
        joy.setLastPress(millis());
        letterLocked = true;
        modifyLetter(letterIndex);
      }
    }
    else if (joy.returnedInDefaultPosition()) {
      joy.setDefaultPosition(true);
    }
  }
}

void goBackToMainMenu() {
  lcd.clear();
  displayMainMenu();
  drawArrow(mainOptions[currentMainOptionIndex][0], mainOptions[currentMainOptionIndex][1]);
}

void settingsMenuMoves() {
  joy.setValueSW(digitalRead(pinSW));
  joy.setValueX(analogRead(pinX));
  joy.setValueY(analogRead(pinY));

  if (joy.getDefaultPosition()) {
    if (settingsMenuLocked) {
      if (currentSettingsOptionIndex == 0) {
        // settingsMenu locked on Startlevel
        clearMatrix();
        loadLevel(startLevelValue - 1);
        modifyStartLevel();
      }
      else if (currentSettingsOptionIndex == 1) {
        // settingsMenu locked on Name
        modifyName();
      }
    }
    else {
      if (joy.movedUp() or joy.movedDown()) {
        moveToNextOption(settingsOptions, &currentSettingsOptionIndex, settingsOptionsNo);
        joy.setDefaultPosition(false);
      }
      else if (joy.movedLeft()) {
        // go back to main menu
        mainMenuLocked = false;
        while (!joy.returnedInDefaultPosition()) {
          joy.setValueX(analogRead(pinX));
          joy.setValueY(analogRead(pinY));
        }
      }
      else if (joy.pressed() and millis() - joy.getLastPress() >= joy.getPressingInterval()) {
        joy.setLastPress(millis());
        settingsMenuLocked = true;
      }
    }
  }
  else if (joy.returnedInDefaultPosition()) {
    joy.setDefaultPosition(true);
  }
}

void mainMenuMoves() {
  if (joy.movedRight()) {
    moveToNextOption(mainOptions, &currentMainOptionIndex, mainOptionsNo);
    joy.setDefaultPosition(false);
  }
  else if (joy.movedLeft()) {
    moveToPreviousOption(mainOptions, &currentMainOptionIndex, mainOptionsNo);
    joy.setDefaultPosition(false);
  }
  else if (joy.pressed() and millis() - joy.getLastPress() >= joy.getPressingInterval()) {
    joy.setLastPress(millis());
    mainMenuLocked = true;
  }
}

void createLevelFrame() {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      levelFrame[row][col] = levels[currentLevel - 1][row][col];
}

void loadLevelFrame() {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      if (levelFrame[row][col])
        lc.setLed(0, row, col, true);
}

void loadLevel(int levelIndex) {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      if (levels[levelIndex][row][col])
        lc.setLed(0, row, col, true);
}

void clearMatrix() {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      lc.setLed(0, row, col, false);
}

int randomColDirection() {
  int aux = rand();
  if (aux % 3 == 0)
    return STRAIGHT;
  else if (aux % 3 == 1)
    return LEFT;
  else
    return RIGHT;
}

int randomLeftRightCol() {
  int aux = rand();
  if (aux % 2 == 1)
    return LEFT;
  else
    return RIGHT;
}

void destroyFlameRight() {
  levelFrame[flameRight.row][flameRight.col] = 0;
  lc.setLed(0, flameRight.row, flameRight.col, false);
  flameRight.destroyed = true;
}

void destroyFlameLeft() {
  levelFrame[flameLeft.row][flameLeft.col] = 0;
  lc.setLed(0, flameLeft.row, flameLeft.col, false);
  flameLeft.destroyed = true;
}

void restoreBallPaddle() {
  levelFrame[ball.row][ball.col] = 0;
  lc.setLed(0, ball.row, ball.col, false);
  ball.row = 6;
  ball.col = 3;
  levelFrame[ball.row][ball.col] = 1;
  lc.setLed(0, ball.row, ball.col, true);

  levelFrame[7][paddle.left] = 0;
  lc.setLed(0, 7, paddle.left, false);
  levelFrame[7][paddle.center] = 0;
  lc.setLed(0, 7, paddle.center, false);
  levelFrame[7][paddle.right] = 0;
  lc.setLed(0, 7, paddle.right, false);
  paddle.left = 2;
  paddle.center = 3;
  paddle.right = 4;
  levelFrame[7][paddle.left] = 1;
  lc.setLed(0, 7, paddle.left, true);
  levelFrame[7][paddle.center] = 1;
  lc.setLed(0, 7, paddle.center, true);
  levelFrame[7][paddle.right] = 1;
  lc.setLed(0, 7, paddle.right, true);
}

void makeMove() {
  levelFrame[ball.row][ball.col] = 0;
  lc.setLed(0, ball.row, ball.col, false);
  ball.row += rowDirection;
  ball.col += colDirection;
  levelFrame[ball.row][ball.col] = 1;
  lc.setLed(0, ball.row, ball.col, true);
}

void changeDirections(bool brickCollision) {
  if (brickCollision == true) {
    tone(buzzerPin, 50, 100);
    destroyedBricks++;
    score += 5;
    updateScore();
    if (destroyedBricks % 5 == 0 && waitBeforeBallMove > MIN_TIME_BEFORE_MOVE)
      waitBeforeBallMove -= decreaseTime;
    rowDirection = -rowDirection;
    colDirection = randomLeftRightCol();
  }
  if (ball.row + rowDirection < 0 || ball.row + rowDirection > 7) {
    rowDirection = -rowDirection;
    if (colDirection == STRAIGHT)
      colDirection = randomColDirection();  // to avoid straight loop
  }
  if (ball.col + colDirection < 0 || ball.col + colDirection > 7)
    colDirection = -colDirection;
}

void initializeFlames() {
  flameLeft.row = 6;
  flameLeft.col = paddle.left;
  flameLeft.destroyed = false;
  flameRight.row = 6;
  flameRight.col = paddle.right;
  flameRight.destroyed = false;

  levelFrame[flameLeft.row][flameLeft.col] = 1;
  lc.setLed(0, flameLeft.row, flameLeft.col, true);
  levelFrame[flameRight.row][flameRight.col] = 1;
  lc.setLed(0, flameRight.row, flameRight.col, true);
}

void moveFlame(Flame *flame) {
  levelFrame[flame->row][flame->col] = 0;
  lc.setLed(0, flame->row, flame->col, false);
  flame->row += UP;
  levelFrame[flame->row][flame->col] = 1;
  lc.setLed(0, flame->row, flame->col, true);
}

int verifyFlameCollisions(Flame *flame, String type) {
  if (flame->row + UP == ball.row && flame->col == ball.col) { // flame hits ball
    loseLife();
    return -1;
  }

  if (flame->row == 0) {
    if (type == "left")
      destroyFlameLeft();
    else if (type == "right")
      destroyFlameRight();
  }
  else if (levelFrame[flame->row + UP][flame->col] == 1) { // flame hits brick
    // delete brick
    levelFrame[flame->row + UP][flame->col] = 0;
    lc.setLed(0, flame->row + UP, flame->col, false);
    tone(buzzerPin, 50, 100);
    destroyedBricks++;
    score += 5;
    updateScore();
    if (destroyedBricks % 5 == 0 && waitBeforeBallMove > MIN_TIME_BEFORE_MOVE)
      waitBeforeBallMove -= decreaseTime;
    if (type == "left")
      destroyFlameLeft();
    else if (type == "right")
      destroyFlameRight();
  }

  return 0;
}

int moveFlames() {
  if (!flameLeft.destroyed) {
    if (verifyFlameCollisions(&flameLeft, "left") == -1) return -1;
  }

  if (!flameRight.destroyed) {
    if (verifyFlameCollisions(&flameRight, "right") == -1) return -1;
  }

  if (!flameLeft.destroyed)
    moveFlame(&flameLeft);
  if (!flameRight.destroyed)
    moveFlame(&flameRight);

  return 0;
}

void handleDirectPaddleCollision() {
  if (ball.col == 7 || ball.col == 0) {
    // ball fell in the corner of the matrix => col direction stays the same
  }
  else if (levelFrame[ball.row + rowDirection][ball.col - 1] == 0) { // ball is to the left of the pallet
    colDirection = LEFT;
  }
  else if (levelFrame[ball.row + rowDirection][ball.col + 1] == 0) { // ball is to the right of the pallet
    colDirection = RIGHT;
  }
  else colDirection = STRAIGHT;  // ball is to the center of the pallet
}

int moveBall() {
  if ((!flameLeft.destroyed && flameLeft.row == ball.row + rowDirection && flameLeft.col == ball.col + colDirection)
      || (!flameRight.destroyed && flameRight.row == ball.row + rowDirection && flameRight.col == ball.col + colDirection)) {
    makeMove();
    loseLife();

    return -1;
  }

  bool brickCollision = false;

  if (ball.row + rowDirection == 7) {
    if (levelFrame[ball.row + rowDirection][ball.col] == 1) // direct paddle collision occured
      handleDirectPaddleCollision();
    else if (levelFrame[ball.row + rowDirection][ball.col + colDirection] == 1) // lateral paddle collision occured
      colDirection = -colDirection;
    else if (levelFrame[ball.row + rowDirection][ball.col + colDirection] == 0) { // bottom wall collision occured
      makeMove();
      loseLife();
      return -1;
    }

    rowDirection = -rowDirection;
  }

  if (levelFrame[ball.row + rowDirection][ball.col + colDirection] == 1) // brick collision occured
    brickCollision = true;

  makeMove();
  changeDirections(brickCollision);
  return 0;
}

void movePaddleLeft() {
  if (paddle.left > 0) {
    levelFrame[7][paddle.right] = 0;
    lc.setLed(0, 7, paddle.right, false);
    paddle.left--;
    paddle.center--;
    paddle.right--;
    levelFrame[7][paddle.left] = 1;
    lc.setLed(0, 7, paddle.left, true);
  }
}

void movePaddleRight() {
  if (paddle.right < 7) {
    levelFrame[7][paddle.left] = 0;
    lc.setLed(0, 7, paddle.left, false);
    paddle.left++;
    paddle.center++;
    paddle.right++;
    levelFrame[7][paddle.right] = 1;
    lc.setLed(0, 7, paddle.right, true);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);

  lcd.begin(16, 2);

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 0); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  rowDirection = UP;
  colDirection = STRAIGHT;

  lcd.createChar(1, heart);   // Creating specified character in CG-RAM
  lcd.createChar(2, arrowDown1);
  lcd.createChar(3, arrowDown2);
  lcd.createChar(4, arrowUp1);
  lcd.createChar(5, arrowUp2);
  lcd.createChar(6, arrowUp);

  song();

  lcd.setCursor(0, 0);
  lcd.print("Brick Braker");
  lcd.setCursor(0, 1);
  lcd.print("X - mas Edition");
  delay(2000); // wait for the welcome message to be displayed two seconds

  displayMainMenu();
  drawArrow(mainOptions[currentMainOptionIndex][0], mainOptions[currentMainOptionIndex][1]);
}

void loop() {
  joy.setValueSW(digitalRead(pinSW));
  joy.setValueX(analogRead(pinX));
  joy.setValueY(analogRead(pinY));

  if (joy.getDefaultPosition()) {
    mainMenuMoves();

    if (mainMenuLocked) {
      // mainMenuLocked on currentMainOptionIndex --> go to selected next menu
      if (currentMainOptionIndex == 0)
        goToPlayMenu();
      else if (currentMainOptionIndex == 1)
        goToHiScoreMenu();
      else if (currentMainOptionIndex == 2)
        goToSettingsMenu();
      else if (currentMainOptionIndex == 3)
        goToInfoMenu();

      goBackToMainMenu();
    }
  }
  else if (joy.returnedInDefaultPosition()) {
    joy.setDefaultPosition(true);
  }
}
