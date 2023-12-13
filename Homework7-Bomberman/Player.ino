
void blinkPlayer() {
  if (millis() - lastPlayerBlink >= playerBlinkTime) {
    playerState = !playerState;
    lc.setLed(0, playerLine, playerColumn, playerState);
    lastPlayerBlink = millis();
  }
}


void movePlayer() {
  if (currentMove != -1) {

    
    //bool inPortal = checkPlayerInPortal();

    //move into the second room
    if (currentMove == moves[RIGHT] && currentMatrix == matrix1 && playerLine == matrix1PortalLine && playerColumn == matrix1PortalColumn) {
      playerLine = matrix2PortalLine;
      playerColumn = matrix2PortalColumn;
      currentMatrix = matrix2;

    }

    //move into the first room
    else if (currentMove == moves[LEFT] && currentMatrix == matrix2 && playerLine == matrix2PortalLine && playerColumn == matrix2PortalColumn) {
      playerLine = matrix1PortalLine;
      playerColumn = matrix1PortalColumn;
      currentMatrix = matrix1;

    }

    else {
      byte newPlayerLine = playerLine + directionMatrix[currentMove][0];
      byte newPlayerColumn = playerColumn + directionMatrix[currentMove][1];

      //we let the player move into the new position if there is no wall, if he is in the limits of the matrix or if there are bombs
      if ((currentMatrix[newPlayerLine][newPlayerColumn] == 0 || currentMatrix[newPlayerLine][newPlayerColumn] == 2) && (newPlayerLine >= 0 && newPlayerLine <= matrixSize - 1 && newPlayerColumn >= 0 && newPlayerColumn <= matrixSize - 1)) {
        //we first turn off the last player position
        lc.setLed(0, playerLine, playerColumn, 0);
        playerLine = newPlayerLine;
        playerColumn = newPlayerColumn;
      }
    }

    //in the end we ensure that we mark the player as static
    currentMove = -1;
  }
}


void checkPlayerInPortal(){

 

}


void checkTimePass() {
  if (millis() - lastTimeCount >= timeDelay && !stopTimeCount) {
    lastTimeCount = millis();
    time--;

    //if we reached the end of time the game ends
    if (time == 0) {
      //we force the entering of the condition when the player is dead because the effects are the same
      lives = 0;
      checkPlayerAlive();
    }
  }
}

void checkLevelRules() {


  switch (level) {
    case 1:
      {
        if (inGame) {
          firstLevelRules();
        } else {
          prepareLevel2();
        }
      }
      break;
    case 2:
      {
        secondLevelRules();
      }
      break;
    case 3:
      {
        thirdLevelRules();
      }
      break;
  }
}

void firstLevelRules() {

  score = noOfWalls - wallsCoverage;
  for (int index = 0; index < realNoOfEnemies; index++) {
    enemies[index].moveEnemy();
  }

  if (wallsCoverage == 0) {
    score += time;
    stopTimeCount = true;
    inGame = false;
    shown = false;
    prepareLevel2();
  }
  checkScore();
}

void prepareLevel2() {
  blinkArrow();
  nextLevelMessage();

  if (playerLine == matrix1PortalLine && playerColumn == matrix1PortalColumn) {
    currentMatrix = matrix2;  
    //resetGame();
    wallsCoverage = noOfWallsLevel2;
    inGame = true;
    stopTimeCount = false;
    time = startTime;
    lives = noOfLives;
    level = 2;
    playerLine = 0;
    playerColumn = 0;
    oldScore = score;
  }
}



void nextLevelMessage() {
  if (!shown) {
    shown = true;
    lcd.clear();
  }
  lcd.setCursor(0, 0);
  lcd.print(F("Follow the arrow"));
  lcd.setCursor(0, 1);
  lcd.print(F("to the next room"));
}


void secondLevelRules() {
  score = computeLiveScore();

  if (wallsCoverage == 0 && realNoOfEnemies == 0) {
    score += time;
    stopTimeCount = true;
    inGame = false;
    shown = false;
  }
  checkScore();
}


int computeLiveScore() {
  return oldScore + enemyPoints * (initialNoOfEnemies - realNoOfEnemies) + noOfWallsLevel2 - wallsCoverage;
}

void thirdLevelRules() {
}

void checkPlayerDamage(byte minLine, byte maxLine, byte minColumn, byte maxColumn, byte bombLine, byte bombColumn) {

  if ((playerLine <= maxLine && playerLine >= minLine && playerColumn == bombColumn) || (playerColumn <= maxColumn && playerColumn >= minColumn && playerLine == bombLine)) {
    lives--;
  }

  //actionare arie de patrat
  // if (playerLine <= maxLine && playerLine >= minLine && playerColumn <= maxColumn && playerColumn >= minColumn) {
  //   lives--;
  // }
  checkPlayerAlive();
}

void checkPlayerAlive() {
  if (lives == 0) {
    isAlive = false;
    // startGreet = 0;
    // greeted = false;
    shown = false;
    inMenu = true;
    // currentMenu = 1;
    currentOption = 0;
    checkGameRestart();
  }
}

//the score represents the number of distroyed walls and the number of seconds left until the time has gone
void checkScore() {
  //we memorate the hall of fame before each game iteration
  byte first = EEPROM.read(firstPlayerMemoryLocation + maxNameCharacters);
  byte second = EEPROM.read(secondPlayerMemoryLocation + maxNameCharacters);
  byte third = EEPROM.read(thirdPlayerMemoryLocation + maxNameCharacters);
  // Serial.println(third);

  if (score > first) {
    displayHighscoreMessage();
  } else if (score > second) {
    podiumMessage();
  } else if (score > third) {
    podiumMessage();
  }
}


void checkEepromScore() {
  byte first = EEPROM.read(firstPlayerMemoryLocation + maxNameCharacters);
  byte second = EEPROM.read(secondPlayerMemoryLocation + maxNameCharacters);
  byte third = EEPROM.read(thirdPlayerMemoryLocation + maxNameCharacters);
  if (score > first) {
    updateEepromScore(firstPlayerMemoryLocation);
  } else if (score > second) {
    updateEepromScore(secondPlayerMemoryLocation);
  } else if (score > third) {
    updateEepromScore(thirdPlayerMemoryLocation);
  }
}

//BE CAREFUL IT'S COMMENTED
void updateEepromScore(byte memoryLocation) {
  // EEPROM.put(memoryLocation, name);
  // EEPROM.update(memoryLocation + maxNameCharacters, score);
}

void displayHighscoreMessage() {
  if (!highscoreDisplayed) {
    if (!lastMessageDisplay) {
      lastMessageDisplay = millis();
    }

    if (millis() - lastMessageDisplay >= messageDisplayTime) {
      anotherMessage = false;
      podiumDisplayed = true;
      lastMessageDisplay = 0;
    }

    else {
      anotherMessage = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You've just set");
      lcd.setCursor(0, 1);
      lcd.print("a new highscore");
    }
  }
}

void podiumMessage() {

  if (!podiumDisplayed) {
    if (lastMessageDisplay == 0) {
      lastMessageDisplay = millis();
    }

    if (millis() - lastMessageDisplay >= messageDisplayTime) {
      anotherMessage = false;
      podiumDisplayed = true;
      lastMessageDisplay = 0;
    }

    else {
      anotherMessage = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("You're on the"));
      lcd.setCursor(0, 1);
      lcd.print(F("podium.Congrats!"));
    }
  }
}