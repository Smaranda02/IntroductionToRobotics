
void loop() {

  if (gameStarted) {
    if (isAlive) {

      readBrightnessValue();

      readJoystickValues();
      checkDirection();

      movePlayer();

      checkButtonPress();

      for (int index = 0; index <= maximumBombs; index++) {
        if (bombs[index].isDetonated == false && bombs[index].bombCoutdownStart != 0) {
          bombs[index].checkBombState();
        }
      }

      lightUpMatrix();
      blinkPlayer();
    } else {
      checkButtonPress();
      displaySadFace();
    }
  }

  //we are navigating through the menu and we haven't pressed the Start Game option yet
  else {
    
  }
}
