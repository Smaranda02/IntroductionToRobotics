
void lightUpMatrix() {

  if (wallsCoverage > 0) {
    for (int line = 0; line < matrixSize; line++)
      for (int column = 0; column < matrixSize; column++) {
        //we light the current LED only if it's different from the player and the bomb
        if ((column != playerColumn || line != playerLine) && currentMatrix[line][column] != 2 && currentMatrix[line][column] != 3) {
          lc.setLed(0, line, column, currentMatrix[line][column]);
        }
      }
  }

  else {
    blinkArrow();
  }
}



void resetMatrix() {
  for (int line = 0; line < matrixSize; line++)
    for (int column = 0; column < matrixSize; column++) {
      matrix1[line][column] = 0;
      matrix2[line][column] = 0;
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

void generateWalls() {
  for (int index = 0; index < 2 * wallsCoverage; index += 2) {
    int randomLine1 = random(0, 8);
    int randomColumn1 = random(0, 8);

    int randomLine2 = random(0, 8);
    int randomColumn2 = random(0, 8);

    volatile bool isWall1Ok;
    volatile bool isWall2Ok;

    isWall1Ok = checkGeneratedWall(randomLine1, randomColumn1);
    isWall2Ok = checkGeneratedWall(randomLine2, randomColumn2);

    if (isWall1Ok) {
      matrix1[randomLine1][randomColumn1] = 1;
    } else {
      index--;
    }

    if (isWall2Ok) {
      matrix2[randomLine2][randomColumn2] = 1;
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