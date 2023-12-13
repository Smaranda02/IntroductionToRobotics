#include "LedControl.h"
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "Matrix.h"
#include "Bombs.h"
#include "Joystick.h"
#include "LCD.h"
#include "Menu.h"
#include "Player.h"
#include "Buzzer.h"
#include "Enemy.h"


const int maximumBombs = 4;
Bomb bombs[maximumBombs];
int bombsNumber = -1;

//arrow
const byte arrowSize = 4;
byte Arrow[arrowSize][2] = {
  { 3, 6 },
  { 4, 6 },
  { 4, 7 },
  { 5, 6 }
};
unsigned long lastArrowBlink = 0;
volatile bool arrowState = LOW;

bool gameStarted = false;


void checkButtonPress() {
  if (isButtonPressed) {
    //we consider the button press as placing a bomb only if the player is alive and there isn't already a bomb (matrix[i][j] != 2)
    if (isAlive) {
      if (currentMatrix[playerLine][playerColumn] == 0) {
        for (int i = 0; i < maximumBombs; i++) {
          if (bombs[i].isDetonated) {
            bombs[i] = Bomb(playerLine, playerColumn, millis(), false, 0);
            bombs[i].isDetonated = false;
            bombsNumber++;
            break;
          }
        }

        //we mark with 2 the existence of a bomb in the matrix
        currentMatrix[playerLine][playerColumn] = 2;
      }
    }
    isButtonPressed = false;
  }
}

void generateWalls(byte (*matrix)[matrixSize], byte size);
void resetMatrix(byte (*matrix)[matrixSize] ) ;

void resetGame() {
  isAlive = true;
  lives = noOfLives;
  wallsCoverage = rooms[level - 1].walls;
  playerLine = 0;
  playerColumn = 0;
  bombsNumber = -1;
  time = startTime;

  //we regenerate walls for the room corresponding to the current level
  if (level == 3) {
    //to be continued
    resetMatrix(*matrixPointers[level - 1]);
    resetMatrix(*matrixPointers[level]);
    generateWalls(*matrixPointers[level - 1], rooms[level - 1].walls);  //for room 3
    generateWalls(*matrixPointers[level], rooms[level].walls);          //for room 4

  } else {
    resetMatrix(*matrixPointers[level - 1]);
    generateWalls(*matrixPointers[level - 1], rooms[level - 1].walls);  //level 1 means index 0 in our array of pointers
  }
}



void handleInterrupt() {

  static unsigned long interruptTime = 0;  // Retains its value between calls
  interruptTime = micros();                // Get the current time in microseconds

  // If interrupts come faster than the debounce delay, ignore them
  if (interruptTime - lastInterruptTime > debounceDelay * 1000) {  // Convert debounceDelay to microseconds for comparison
    isButtonPressed = true;
  }

  lastInterruptTime = interruptTime;
}
