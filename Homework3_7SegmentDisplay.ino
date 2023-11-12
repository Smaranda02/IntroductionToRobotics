// declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
const int E = 4;
const int F = 5;
const int G = 6;
const int DP = 7;


const int possibleMoves = 4;
bool commonAnode = false;
const int noOfLeds = 8;

int segmentsPins[noOfLeds] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int segmentOrder[noOfLeds] = { A, B, C, D, E, F, G, DP };

const int noOfMoves = 4;
int moves[noOfMoves] = {
  UP, DOWN, LEFT, RIGHT
};

int ledStates[noOfLeds] = { 0, 0, 0, 0, 0, 0, 0, 1 };


int currentMove = -1;
int currentSegment = segmentOrder[DP];

const int buzzerPin = 13;
int buzzerTone = 1000;
int buzzerDuration = 1000;


const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A1;  // A0 - analog pin connected to X output
const int pinY = A0;  // A1 - analog pin connected to Y output
byte swState = LOW;
int xValue = 0;
int yValue = 0;


// a = first line
//...
// g = 7th line
// dp = 8th line
// UP = first column
// DOWN = second column
// LEFT = third column
// RIGHT = fourth column
int movesMatrix[noOfLeds][possibleMoves] = {
  { -1, 6, 5, 1 },
  { 0, 6, 5, -1 },
  { 6, 3, 4, 7 },
  { 6, -1, 4, 2 },
  { 6, 3, -1, 2 },
  { 0, 6, -1, 1 },
  { 0, 3, -1, -1 },
  { -1, -1, 2, -1 }
};

bool joyMoved = false;
int minThreshold = 200;
int maxThreshold = 800;

int idleMin = 510;
int idleMax = 550;

int flickerTime = 500;
long lastFlicker = 0;

bool flickerLedState = 1;

const unsigned long debounceDelay = 200;  // Debounce time in milliseconds

const int resetTime = 2000;

volatile bool swPressed = false;
volatile bool reset = false;         // Flag to indicate button press event
unsigned long lastDebounceTime = 0;  // Keeps track of the last time the button was pressed
volatile unsigned long lastLongPress = 0;

volatile bool possiblePress = false;
volatile bool shouldStartCounter = false;

bool buttonReading = HIGH;  // The current reading from the button pin
bool buttonState = HIGH;    // The previous state of the button (HIGH when not pressed to INPUT_PULLUP)

bool possibleReset = false;
unsigned long possibleLongPressTime = 0;

void setup() {

  pinMode(pinSW, INPUT_PULLUP);  // activate pull-up resistor on the // push-button pin
  // Start the serial communication.
  Serial.begin(9600);

  pinMode(buzzerPin,  OUTPUT);

  // initialize all the pins
  for (int i = 0; i < noOfLeds; i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING);
}



void loop() {

  //We read the values for each axis from the joystick
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  //We set the currentMove to -1 in order to interpret each joyStick move only a single time  
  currentMove = -1;
  
  //this function makes the selected segment to blink
  flicker();
  checkPossiblePress();
  checkPossibleReset();
  //this function interprets the joystick move as a right, left, up or down
  checkDirection();
  //this function prints the led states
  displaySegment(currentMove);
}


void displaySegment(int currentMove) {

  int line = currentSegment;
  int nextSegment = -1;
  if (currentMove != -1) {
    nextSegment = movesMatrix[line][currentMove];
  }

  if (nextSegment != -1) {
    currentSegment = nextSegment;
  }

  for (int i = 0; i < noOfLeds; i++) {
    if (currentSegment != i) {
      digitalWrite(segmentsPins[i], ledStates[i]);
    }
  }
}


void checkPossiblePress(){
  if (possiblePress) {
    // Start the debounce counter if needed
    if (shouldStartCounter) {
      lastDebounceTime = millis();
      possibleLongPressTime = millis();
      shouldStartCounter = false;
    }

    // If the debounce period has elapsed
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // Read the current state of the button
      buttonReading = digitalRead(pinSW);
      // If the button state has changed (it was not a false press)
      if (buttonReading != buttonState) {
        // Toggle the LED state if the button was truly pressed (read as LOW due to INPUT_PULLUP)
        if (buttonReading == LOW) {
          ledStates[currentSegment] = !ledStates[currentSegment];
          possibleReset = true;
        }

      }  // If the states match, it was a false alarm due to noise
      // Reset the possiblePress flag
      possiblePress = false;
    }
  }
}


void checkPossibleReset(){
   if (possibleReset) {
    //The button was pressed for long enough to to consider it as a reset 
    if (millis() - possibleLongPressTime > resetTime) {

      buttonReading = digitalRead(pinSW);
      if (buttonReading != buttonState) {
        //The button is pressed
        if (buttonReading == LOW) {

          //We turn off the leds for each segment and we set the selected segment to be the DP
          for (int i = 0; i < noOfLeds; i++) {
            ledStates[i] = 0;
          }
          currentSegment = segmentOrder[DP];
          tone(buzzerPin,buzzerTone,buzzerDuration);
        }
        possibleReset = false;
      }
    }
  }
}


void checkDirection(){
  
  //Right direction
  if (xValue > maxThreshold && joyMoved == false) {

    if (yValue < maxThreshold && yValue > minThreshold) {
      currentMove = moves[RIGHT];

    }

    else if (yValue >= maxThreshold) {
      if (xValue > yValue) {
        //the joyStick is more on the right than it is upwards
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (yValue <= minThreshold) {
      //the joyStick is more on the right than it is downwards
      //the joyStick is closer to the threshold for the right than the threashold for the down
      if (maxThreshold - xValue < yValue - minThreshold) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Left direction
  if (xValue < minThreshold && joyMoved == false) {

    if (yValue < maxThreshold && yValue > minThreshold) {
      currentMove = moves[LEFT];
    } else if (yValue >= maxThreshold) {
      if (xValue - minThreshold < maxThreshold - yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (yValue <= minThreshold) {
      if (xValue < yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Down direction
  if (yValue < minThreshold && joyMoved == false) {

    if (xValue < maxThreshold && xValue > minThreshold) {
      currentMove = moves[DOWN];
    } else if (xValue >= maxThreshold) {
      if (maxThreshold - xValue < yValue - minThreshold) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    else if (xValue <= minThreshold) {
      if (xValue < yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Up direction
  if (yValue > maxThreshold && joyMoved == false) {

    if (xValue < maxThreshold && xValue > minThreshold) {
      currentMove = moves[UP];
    } else if (xValue >= maxThreshold) {
      if (xValue > yValue) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (xValue <= minThreshold) {
      if (xValue - minThreshold < maxThreshold - yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[UP];
      }
    }

    joyMoved = true;
  }

  if (xValue >= idleMin && xValue <= idleMax && yValue >= idleMin && yValue <= idleMax) {
    joyMoved = false;
  }

}


void handleInterrupt() {

  possiblePress = true;
  shouldStartCounter = true;

}


void flicker() {

  //If the selected segment has stayed in one state for long enough we switch its state
  if (millis() - lastFlicker > flickerTime) {

    flickerLedState = !flickerLedState;
    digitalWrite(segmentsPins[currentSegment], flickerLedState);
    lastFlicker = millis();
  }
}
