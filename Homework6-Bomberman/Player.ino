

void blinkPlayer() {
  if (millis() - lastPlayerBlink >= playerBlinkTime) {
    playerState = !playerState;
    lc.setLed(0, playerLine, playerColumn, playerState);
    lastPlayerBlink = millis();
  }
}


void movePlayer() {
  if (currentMove != -1) {

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



void checkPlayerDamage(byte minLine, byte maxLine, byte minColumn, byte maxColumn) {
  if (playerLine <= maxLine && playerLine >= minLine && playerColumn <= maxColumn && playerColumn >= minColumn) {
    lives--;
  }
  checkPlayerAlive();
}

void checkPlayerAlive() {
  if (lives == 0) {
    isAlive = false;
  }
}
