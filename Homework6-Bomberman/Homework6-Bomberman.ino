#include "LedControl.h"
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "Bombs.h"
#include "Joystick.h"
#include "Matrix.h"
#include "LCD.h"

//buzzer
const byte buzzerPin = 8;
unsigned int buzzerTone = 1000;
unsigned int buzzerDuration = 500;

//player variables
unsigned int playerBlinkTime = 400;
unsigned long lastPlayerBlink = 0;
volatile bool playerState = LOW;
byte noOfLives = 3;
byte lives = noOfLives;
volatile bool isAlive = true;
byte score = 0;

const byte maxNameCharacters = 5;
byte maxPodiumPlayers = 3;

struct AwardedPlayer {
  const char playerName[maxNameCharacters]="";
  byte highscore;
};

//player position
byte playerLine = 0;
byte playerColumn = 0;


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

bool gameStarted = true;


void checkButtonPress() {
  if (isButtonPressed) {
    //we consider the button press as placing a bomb only if the player is alive and there isn't already a bomb (matrix[i][j] != 2)
    if (isAlive) {
      if (currentMatrix[playerLine][playerColumn] == 0) {
        unsigned long t = millis();
        for (int i = 0; i < maximumBombs; i++) {
          if (bombs[i].isDetonated) {
            bombs[i] = Bomb(playerLine, playerColumn, t, false, 0);
            bombs[i].isDetonated = false;
            bombsNumber++;
            break;
          }
        }

        //we mark with 2 the existence of a bomb in the matrix
        currentMatrix[playerLine][playerColumn] = 2;
      }
    } else {
      resetGame();
    }
    isButtonPressed = false;
  }
}


void resetGame() {
  resetMatrix();
  isAlive = true;
  lives = noOfLives;
  playerLine = 0;
  playerColumn = 0;
  wallsCoverage = noOfWalls;
  generateWalls();
  bombsNumber = -1;
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
