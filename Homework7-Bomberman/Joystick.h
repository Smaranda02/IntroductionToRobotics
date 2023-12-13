// const int pinSW = 13;  // digital pin connected to switch output
const int pinX = A1;   // A1 - analog pin connected to X output
const int pinY = A0;   // A0 - analog pin connected to Y output
byte swState = LOW;
int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int minThreshold = 200;
int maxThreshold = 800;

int idleMin = 510;
int idleMax = 550;

// external button
const unsigned long debounceDelay = 200;  // Debounce time in milliseconds
const byte buttonPin = 2;
volatile bool isButtonPressed = false;
unsigned long lastInterruptTime = 0;

//joystick moves
const byte UP = 0;
const byte DOWN = 1;
const byte LEFT = 2;
const byte RIGHT = 3;

const byte noOfMoves = 4;
byte moves[noOfMoves] = {
  UP, DOWN, LEFT, RIGHT
};

int currentMove = -1;