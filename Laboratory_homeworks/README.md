# Laboratory Homeworks - Introduction to Robotics Courses @ University of Bucharest (Unibuc Robotics)

# 1st Homework
**3PotentiometersRGB**: Controlling the colors of the RGB LED by reading the value of the potentiometers and mapping each one to a color: R, G, or B.

# 2nd Homework
**knockDetector**: Read the value of a piezo buzzer and if it is higher than a specified threshold, start a buzzer sound (using a different buzzer) which stops at the press of a pushbutton.

# 3nd Homework
**4DigitDisplayJoystickControl**: Individually controlling each digit of a 4 digit 7-segment display with a joystick: 
- Using the X-axis, cycle through the digits and chose which one to modify (blink the DP led of the current position).
- Press the button to lock on the selected digit (keep the DP lit while selected digit is locked).
- After that, use the Y-axis to modify the value of the digit, both up (increase) and down (decrease). DP should remain lit while executing this step.
- Press the button again to unlock the selection and be able to cycle through the 4 digits again (DP should return to the blinking state).

The X and Y axis must be used exclusively. When you cycle through the digits, you cannot modify the value. When you are locked in on a digit, you cannot switch to another one without pressing the button

There are 3 possible states regarding decimal point: locked in (on) / selected - not locked in though (blinking) / not selected (off).
