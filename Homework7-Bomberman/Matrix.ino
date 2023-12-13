
void lightUpMatrix() {

  if (wallsCoverage > 0) {
    for (int line = 0; line < matrixSize; line++)
      for (int column = 0; column < matrixSize; column++) {
        //we light the current LED only if it's different from the player and the bomb
        if ((column != playerColumn || line != playerLine) && currentMatrix[line][column] != 2 && currentMatrix[line][column] != 3 
                  && currentMatrix[line][column] != 4) {
          lc.setLed(0, line, column, currentMatrix[line][column]);
        }
      }
  }

  else {
    blinkArrow();
  }
}



void resetMatrix(byte (*matrix)[matrixSize] ) {
  for (int line = 0; line < matrixSize; line++)
    for (int column = 0; column < matrixSize; column++) {
      matrix[line][column] = 0;
    }
}


void blinkArrow() {
  if (millis() - lastArrowBlink >= playerBlinkTime) {
    arrowState = !arrowState;
    for (int line = 0; line < arrowSize; line++) {
      lc.setLed(0, Arrow[line][0], Arrow[line][1], arrowState);
    }
    lastArrowBlink = millis();
  }
}


void generateWalls(byte (*matrix)[matrixSize] , byte size) {

  for (int index = 0; index < size; index ++) {

    int randomLine = random(0, 8);
    int randomColumn = random(0, 8);

    volatile bool isWallOk;

    isWallOk = checkGeneratedWall(randomLine, randomColumn);

    if (isWallOk) {
      matrix[randomLine][randomColumn] = 1;
    } else {
      index--;
    }
  }
}

bool checkGeneratedWall(int line, int column) {
  //we check we don't create walls around the player
  if (line == playerLine) {
    if (abs(playerColumn - column) == 1 || playerColumn == column) {
      return false;
    }
  } else if (column == playerColumn) {
    if (abs(playerLine - line) == 1 || playerLine == line) {
      return false;
    }
  }
  //we check the LED is not already ON
  else if (currentMatrix[line][column] == 1) {
    return false;
  }

  return true;
}


void displaySadFace() {
  for (int line = 0; line < matrixSize; line++)
    for (int column = 0; column < matrixSize; column++) {
      lc.setLed(0, line, column, sadFace[line][column]);
    }
}