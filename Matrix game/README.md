# Brick Breaker X-mas edition - Matrix game - @UnibucRobotics

## Description

**Brick Breaker** is a game in which the player must smash a wall of bricks by deflecting a bouncing ball with a paddle. The paddle moves horizontally and is controlled with a joystick. The player gets 3 lives to start with. When the ball is bounced up by the paddle, it will hit a brick and then the brick is gone. After that the ball will come down; if the paddle misses the ball, a life is lost. When all the bricks have been destroyed, the player advances to a new, harder level. If all lives are lost, the game is over.

>  **Power ups** implemented in X-mas edition: *the fire laser* (the player can shoot flaming fireballs using the joystick).


![LEDmatrix](https://user-images.githubusercontent.com/56949829/70387377-52f44c00-19ad-11ea-9e6b-c0083fc9a457.gif)


## Meeting the requirements

**Score**: initially the score is 0 and it increases with 5 for every brick destroyed. In addition, for every fireshot the score decreases with 5.

**Progress in difficulty**:

-  *level difficulty*: the complexity of the game increases from one level to another as the layout of the bricks changes.

-  *speed increase*: transition to a faster speed occurs after a number of balls were destroyed (multiple of 5) until the max speed is reached.
  
**2 requirements of choice**:

-  **Levels**: 4 levels with different bricks layouts

![level1](https://user-images.githubusercontent.com/56949829/70380951-9918c380-194b-11ea-8a85-8cbd7294dd19.png)
![level2](https://user-images.githubusercontent.com/56949829/70380976-d54c2400-194b-11ea-8012-7a39f024a906.png)
![level3](https://user-images.githubusercontent.com/56949829/70380908-0ed05f80-194b-11ea-8f8a-7c31ebbd64b5.jpg)
![level4](https://user-images.githubusercontent.com/56949829/70380984-f44ab600-194b-11ea-854f-59494bf9604d.png)

-  **Lives**:  at the beginning of every level the number of lives is 3. The number is decreased with one when the paddle misses the bouncing ball.


## Hardware components

- 8x8 LED matrix

- LCD

- Joystick

- MAX7219 Driver

- active buzzer


## How to play instructions

Before playing, make sure that you entered the Settings menu and set your name accordingly (otherwise you will be just another "anonim"). You can also chose the starting level, but take note that the score associated with the levels you passed will be 0 and this can reduce your chances to obtain a better score and, consequently, to do a high score. 

After all settings have been made, the game can start. Use the joystick to move the paddle accordingly. You can also shoot flaming fireballs by pressing the button of the joystick, but be careful not to kill yourself.

## Game logic

![gameLogic](https://user-images.githubusercontent.com/56949829/70388306-8688a380-19b8-11ea-8ad8-0fc4dc15ab9a.png)


```mermaid

graph LR

A{Play} -- Pause Button --> B(Wait)
B -- Pause Button --> A

A -- Lifes = 0--> C(Game over)
C --> I(Try again?)
I -- Yes --> F(Settings)
I -- No --> D(Exit)
F --> A

A -- End of level --> G(Continue?)
G(Continue?) -- No --> D
G -- Yes --> H(Next level)
H --> A

```

## Picture

## Demo video
