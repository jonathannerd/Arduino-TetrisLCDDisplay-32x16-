#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>
#include <Bounce2.h>

// LED Matrix Pins
#define CLK 11
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false,64);

// Joystick Pins
int joystickX = A8;
int joystickY = A9;
int joystickButton = 2;
int yValue;
int xValue;

Bounce2::Button button;

// Game Dimensions
#define GRID_WIDTH 64
#define GRID_HEIGHT 32

bool grid[GRID_WIDTH][GRID_HEIGHT];  // Game grid

// Gravity variables
long lastMoveTime = 0;
int gravityDelay = 500;
int gravity = 1;
long lastMove = 0;

// Current piece
int currentX = GRID_WIDTH / 2 - 2;
int currentY = 0;
int currentShape = 0, rotation = 0;

// Tetris Shapes
int shapes[7][4][4] = {
  { { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 } },  // I-shape
  { { 1, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } },  // O-shape
  { { 0, 1, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } },  // T-shape
  { { 0, 1, 1, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } },  // S-shape
  { { 1, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } },  // Z-shape
  { { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 } },  // L-shape
  { { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 } }  // J-shape
};

void setup() {
  Serial.begin(9600);
  matrix.begin();
  pinMode(joystickButton, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  button.attach(joystickButton);
  button.interval(5);
  resetGrid();
  spawnPiece();
}

void loop() {
  button.update();
  handleInput();

  // Gravity
  if (millis() - lastMoveTime > gravityDelay) {
    moveRight();
    lastMoveTime = millis();
  }

  renderGrid();
}

// Handle joystick input
void handleInput() {
  yValue = analogRead(joystickY);
  xValue = analogRead(joystickX);

  Serial.println(xValue);
 // Serial.println(yValue);

if (xValue > 800){
     gravity = 2;
  }

else if (yValue > 800) moveDown();

else if (yValue < 200) moveUp();

  else {
    gravity = 1;
  }

  if (button.fell()) rotatePiece();
}

// Reset the grid
void resetGrid() {
  for (int x = 0; x < GRID_WIDTH; x++)
    for (int y = 0; y < GRID_HEIGHT; y++)
      grid[x][y] = false;
}

// Spawn a new piece
void spawnPiece() {
  currentX = 0;
  currentY = GRID_WIDTH / 2 - 2;
  currentShape = random(0, 7);
  rotation = 0;
}

// Gravity
void moveRight() {
  if (!collides(currentX + gravity, currentY, rotation)) currentX += gravity;
  else if (!collides(currentX + 1, currentY, rotation)) currentX += 1;
  else {
    lockPiece();
    clearLines();
    spawnPiece();
  }
}

// Move up
void moveUp() {
  if (millis() - lastMove > 100) {
    if (!collides(currentX, currentY - 1, rotation)) currentY--;
    lastMove = millis();
  }
}

// Move down
void moveDown() {
  if (millis() - lastMove > 100) {
    if (!collides(currentX, currentY + 1, rotation)) currentY++;
    lastMove = millis();
  }
}

// Rotate the piece
void rotatePiece() {
  int newRotation = (rotation + 1) % 4;
  if (!collides(currentX, currentY, newRotation)) rotation = newRotation;
}

// Collision detection
bool collides(int x, int y, int r) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shapes[currentShape][i][j] && (x + i >= GRID_WIDTH || y + j < 0 || y + j >= GRID_HEIGHT || grid[x + i][y + j]))
        return true;
    }
  }
  return false;
}

// Lock the piece into the grid
void lockPiece() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shapes[currentShape][i][j])
        grid[currentX + i][currentY + j] = true;
    }
  }
}

// Clear lines
void clearLines() {
  for (int x = 0; x < GRID_WIDTH; x++) {
    bool fullLine = true;
    for (int y = 0; y < GRID_HEIGHT; y++) {
      if (!grid[x][y]) fullLine = false;
    }
    if (fullLine) {
      for (int k = x; k > 0; k--) {
        for (int y = 0; y < GRID_HEIGHT; y++) grid[k][y] = grid[k - 1][y];
      }
    }
  }
}

// Render the grid
void renderGrid() {
  matrix.fillScreen(0);
  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      if (grid[x][y]) matrix.drawPixel(x, y, matrix.Color333(0, 7, 0));  // Locked blocks (green)
    }
  }

  // Draw the current piece
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shapes[currentShape][i][j]) {
        matrix.drawPixel(currentX + i, currentY + j, matrix.Color333(7, 0, 0));  // Falling piece (red)
      }
    }
  }
  matrix.swapBuffers(true);
}
