# Brick Breaker X-mas edition - Matrix game - @UnibucRobotics

## Description

**Brick Breaker** is a game in which the player must smash a wall of bricks by deflecting a bouncing ball with a paddle. The paddle moves horizontally and is controlled with a joystick. The player gets 3 lives to start with. When the ball is bounced up by the paddle, it will hit a brick and then the brick is gone. After that the ball will come down; if the paddle misses the ball, a life is lost. When all the bricks have been destroyed, the player advances to a new, harder level. If all lives are lost, the game is over.

>  **Power ups** implemented in X-mas edition: *the fire laser* (the player can shoot flaming fireballs using the press button of the joystick).

![demo](https://user-images.githubusercontent.com/56949829/71327547-905eea80-2512-11ea-98b8-d528d649f519.gif)


## Meeting the requirements

**Score**: initially the score is 0 and it increases with 5 for every brick destroyed. In addition, for every fireshot the score decreases with 5.

**Progress in difficulty**:

-  *level difficulty*: the complexity of the game increases from one level to another as the layout of the bricks changes.

-  *speed increase*: transition to a faster speed occurs after a number of bricks were destroyed (multiple of 5) until the max speed is reached.
  
**2 requirements of choice**:

-  **Levels**: 4 levels with different bricks layouts

![level1](https://user-images.githubusercontent.com/56949829/71326809-b2ec0600-2508-11ea-87ab-038c5bd88c95.png)
![level2](https://user-images.githubusercontent.com/56949829/71326789-889a4880-2508-11ea-8ff1-46b5a92951b6.png)
![level3](https://user-images.githubusercontent.com/56949829/71326799-95b73780-2508-11ea-9d2b-683a8b47e247.jpg)
![level4](https://user-images.githubusercontent.com/56949829/71326801-a1a2f980-2508-11ea-8daa-c7b5b121e85d.png)

-  **Lives**:  at the beginning of every level the number of lives is 3. The number is decreased with one when the paddle misses the bouncing ball.


## Hardware components

- 8x8 LED matrix

- LCD

- Joystick

- MAX7219 Driver

- active buzzer


## How to play instructions

Before playing, make sure that you entered the Settings menu and set your name accordingly (otherwise you will be just another "ANONIM"). You can also chose the starting level, but take note that the score associated with the levels you passed will be 0 and this can reduce your chances to obtain a better score and, consequently, to do a high score. 

After all settings have been made, the game can start. Use the Ox axis of the joystick to move the paddle to the left or to the right. You can also shoot flaming fireballs by pressing the button of the joystick, but be careful not to kill yourself.

## Game logic
![schematics](https://user-images.githubusercontent.com/56949829/71327709-4f67d580-2514-11ea-8ac1-d5eb7f6db0a8.png)

## Picture
![brickBreaker](https://user-images.githubusercontent.com/56949829/71327257-f184bf00-250e-11ea-8c19-bbfd4d69af88.jpg)

## Demo video
https://drive.google.com/open?id=1XPiIzgEWxzICQadjbLNseWzjOsS-5UtC
