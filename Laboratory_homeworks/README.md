# Laboratory Homeworks - Introduction to Robotics Courses @ University of Bucharest (Unibuc Robotics)

# 1st Homework
**3PotentiometersRGB**: Controlling the colors of the RGB LED by reading the value of the potentiometers and mapping each one to a color: R, G, or B.

# 2nd Homework
**knockDetector**: Read the value of a piezo buzzer and if it is higher than a specified threshold, start a buzzer sound (using a different buzzer) which stops at the press of a pushbutton.

# 3th Homework
**4DigitDisplayJoystickControl**: Individually controlling each digit of a 4 digit 7-segment display with a joystick: 
- Using the X-axis, cycle through the digits and chose which one to modify (blink the DP led of the current position).
- Press the button to lock on the selected digit (keep the DP lit while selected digit is locked).
- After that, use the Y-axis to modify the value of the digit, both up (increase) and down (decrease). DP should remain lit while executing this step.
- Press the button again to unlock the selection and be able to cycle through the 4 digits again (DP should return to the blinking state).

The X and Y axis must be used exclusively. When you cycle through the digits, you cannot modify the value. When you are locked in on a digit, you cannot switch to another one without pressing the button

There are 3 possible states regarding decimal point: locked in (on) / selected - not locked in though (blinking) / not selected (off).

# 4th Homework
**LCDmenuJoystickControl**: a game menu created on a LCD display that can be controlled by a joystick. 

Main menu structure:
- Play: it will show 3 things on the same screen:
    - Lives: 3
    - Level: = startingLevelValue, set up in settings (default is 0). Level gets incremented by 1 every 5 seconds (the score should update accordingly). After 10 seconds, the game is over and "Game over! Press JS to exit" message is displayed on the screen.
    - Score: level * 3
- HiScore: 
    - Initially, we have 0
    - Update it when the game is done. Highest possible score should be achieved by starting at a higher level. The value is saved in eeprom (after restarting the board, the high score remains the same)
- Settings:
    - StartLevel: set the starting level value.
    - Name: displays the name inserted in the serial monitor.

Navigation:
- Scrolling through the main menu is done with the x-axis. The arrow “>” indicates the current option. The joystick moves the arrow indicating/selecting the category in which you want to enter. Entering a category is done by pressing the button (when pressing the button, you enter the option with the arrow next to it).
- Scrolling through the Settings menu is done with the y-axis. Entering a category (Start level/ Name) is done by pressing the button. The starting level is incremented/ decremented using the y-axis.



