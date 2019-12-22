#ifndef joystick_lib
#define joystick_lib

class Joystick {
    int xValue, yValue, swValue;
    const int xRightTreshold = 400;
    const int xLeftTreshold = 600;
    const int yUpTreshold = 400;
    const int yDownTreshold = 600;
    const int pressingInterval = 250;
    unsigned long long lastPress = 0;
    bool inDefaultPosition = true;

  public:
    void setValueSW(int);
    void setValueX(int);
    void setValueY(int);
    void setDefaultPosition(bool);
    void setLastPress(unsigned long);

    unsigned long getLastPress();
    const int getPressingInterval();
    bool getDefaultPosition();

    bool movedLeft();
    bool movedRight();
    bool movedUp();
    bool movedDown();
    bool pressed();
    bool returnedInDefaultPosition();
};

#endif
