#include "joystick.h"

void Joystick::setValueSW(int value) {
  swValue = value;
}

void Joystick::setValueX(int value) {
  xValue = value;
}

void Joystick::setValueY(int value) {
  yValue = value;
}

void  Joystick::setDefaultPosition(bool value) {
  inDefaultPosition = value;
}

bool Joystick::getDefaultPosition() {
  return inDefaultPosition;
}

bool Joystick::movedLeft() {
  return xValue > xLeftTreshold;
}

bool Joystick::movedRight() {
  return xValue < xRightTreshold;
}

bool Joystick::movedUp() {
  return yValue < yUpTreshold;
}

bool Joystick::movedDown() {
  return yValue > yDownTreshold;
}

bool Joystick::pressed() {
  return !swValue;
}

bool Joystick::returnedInDefaultPosition() {
  return ((yValue <= yDownTreshold && yValue >= yUpTreshold) and (xValue <= xLeftTreshold and xValue >= xRightTreshold));
}

void Joystick::setLastPress(unsigned long value) {
  lastPress = value;
}

unsigned long Joystick::getLastPress() {
  return lastPress;
}

const int Joystick::getPressingInterval() {
  return pressingInterval;
}
