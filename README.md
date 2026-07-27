# Arduino RGB Matrix Tetris

**A Tetris-inspired hardware game for an Arduino Mega, a 64×32 RGB LED matrix, and a joystick.**

## What it does

The sketch draws falling tetrominoes directly on the LED matrix. A joystick moves the active piece across the display, its push button rotates the piece, and completed lines are removed from the board.

The matrix is rotated relative to traditional Tetris: gravity advances along the 64-pixel axis, while the player moves across the 32-pixel axis.

## Features

- All seven tetromino shapes
- Random piece spawning
- Joystick movement and soft drop
- Push-button rotation with debouncing
- Grid collision and locked-piece detection
- Completed-line clearing
- Double-buffered RGB matrix rendering

## Hardware

- Arduino Mega
- 64×32 HUB75 RGB LED matrix
- Analog joystick with push button
- Jumper wires and a suitable matrix power supply

## Controls

| Input | Action |
| --- | --- |
| Joystick up/down | Move across the matrix |
| Joystick right | Increase falling speed |
| Joystick press | Rotate clockwise |

## Libraries

Install these through the Arduino Library Manager:

- Adafruit GFX Library
- RGBmatrixPanel
- Bounce2

## Pin configuration

| Function | Arduino pin |
| --- | --- |
| Matrix CLK | `11` |
| Matrix OE | `9` |
| Matrix LAT | `10` |
| Matrix A–D | `A0`–`A3` |
| Joystick X | `A8` |
| Joystick Y | `A9` |
| Joystick button | `2` |

## Upload

1. Clone this repository.
2. Open `TetrisLCDDisplay/TetrisLCDDisplay.ino` in the Arduino IDE.
3. Install the required libraries.
4. Select your Arduino Mega and serial port.
5. Confirm the wiring and upload the sketch.

## Current status

Movement, rotation, collision, locking, rendering, and line clearing are implemented. Scoring, colors per tetromino, and a game-over/restart state would be useful next additions.
