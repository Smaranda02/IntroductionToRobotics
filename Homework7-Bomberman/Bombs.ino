void Bomb::checkBombState() {
  //if there are any bombs
  if (millis() - bombCoutdownStart >= bombDetonationTime) {
    explodeBomb();
  }

  else {
    blinkBomb();
  }
}


void Bomb::blinkBomb() {
  if (millis() - lastBombBlink >= bombBlinkingTime) {
    //we blink the bomb only if the player has moved from that position
    if (playerLine != bombLine || playerColumn != bombColumn) {
      bombState = !bombState;
      lc.setLed(0, bombLine, bombColumn, bombState);
      lastBombBlink = millis();
    }
  }
}

void Bomb::explodeSound() {
  //we let the the buzzer make a sound only if it hasn't buzzed yet and the sound setting is on
  if (!buzzed && soundValue) {
    tone(buzzerPin, buzzerTone, buzzerDuration);
    buzzed = true;
  }
}

void Bomb::explodeBomb() {
  Serial.println(bombLine);
  Serial.println(bombColumn);

  startExplosion = millis();
  explodeSound();
  distroyWalls();
  //we turn off the bomb
  currentMatrix[bombLine][bombColumn] = 0;
  isDetonated = true;
  bombsNumber--;
}

void Bomb::distroyWalls() {
  byte maxLine = min(7, bombLine + actionArea);
  byte minLine = max(0, bombLine - actionArea);
  byte minColumn = max(0, bombColumn - actionArea);
  byte maxColumn = min(7, bombColumn + actionArea);

  //up to down
  for (int line = minLine; line <= maxLine; line++) {

    if (currentMatrix[line][bombColumn] != 2) {
      if (currentMatrix[line][bombColumn] == 1) {
        //we count the damage done only on the walls (not also the bomb itself)
        wallsCoverage--;
      }
      currentMatrix[line][bombColumn] = 0;
    }
  }


  //left to right
  for (int column = minColumn; column <= maxColumn; column++) {
    if (currentMatrix[bombLine][column] != 2) {
      if (currentMatrix[bombLine][column] == 1) {
        wallsCoverage--;
      }
      currentMatrix[bombLine][column] = 0;
    }
  }

  checkPlayerDamage(minLine, maxLine, minColumn, maxColumn, bombLine, bombColumn);
  for(int index = 0; index < realNoOfEnemies; index++){
    enemies[index].checkEnemyDamage(minLine, maxLine, minColumn, maxColumn, bombLine, bombColumn);
  }
 
}
