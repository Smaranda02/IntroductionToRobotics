//added in matrix.ino in light matrix
// in loop verificarea mereu
//i have to update old score
//in movePlayer i have used the portal
//create a level class instead of a room class that has walls and time and enemies

void Enemy::moveEnemy() {
  if (realNoOfEnemies) {
    if (millis() - lastMove >= moveInterval) {
      lastMove = millis();

      byte move = random(0, 4);  //represing UP, DOWN, LEFT, RIGHT

      byte line = enemyLine + directionMatrix[move][0];
      byte column = enemyColumn + directionMatrix[move][1];

      //we check if the current led is a valid position
      if ((currentMatrix[line][column] == 0 || currentMatrix[line][column] == 2)
          && (line >= 0 && line <= matrixSize - 1 && column >= 0 && column <= matrixSize - 1)) {
        currentMatrix[enemyLine][enemyColumn] = 0;  //turn off the old enemy
        currentMatrix[line][column] = 4;            //this is how we mark the enemy
        lc.setLed(0, line, column, 1);              // turn on the current enemy
        //in the end we update the enemy line and column
        enemyLine = line;
        enemyColumn = column;
      }
    }
  }
}

void checkPlayerAlive();

void Enemy::checkPlayerMeeting() {
  if (playerLine == enemyLine && playerColumn == enemyColumn) {
    lives--;
    checkPlayerAlive();
  }
}



void Enemy::checkEnemyDamage(byte minLine, byte maxLine, byte minColumn, byte maxColumn, byte bombLine, byte bombColumn) {

  if ((enemyLine <= maxLine && enemyLine >= minLine && enemyColumn == bombColumn) || (enemyColumn <= maxColumn && enemyColumn >= minColumn && enemyLine == bombLine)) {
    realNoOfEnemies--;
    Serial.println("KILL");
    currentMatrix[enemyLine][enemyColumn] = 0;  //turn off the old enemy
  }
}




bool checkGeneratedEnemy(int line, int column) {

  //we check we don't create an enemy around the player
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


void generateEnemy(byte enemiesNumber) {

  for (int index = 0; index < enemiesNumber; index++) {

    int randomLine = random(0, 8);
    int randomColumn = random(0, 8);

    volatile bool isEnemyOk;

    isEnemyOk = checkGeneratedEnemy(randomLine, randomColumn);

    if (isEnemyOk) {
      currentMatrix[randomLine][randomColumn] = 4;
      enemies[++enemiesIndex] = Enemy(randomLine, randomColumn);
    } else {
      index--;
    }
  }
}
