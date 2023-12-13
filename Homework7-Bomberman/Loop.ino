
void loop() {
  setBrightness(backLightValue);
  if (gameStarted) {

    navigateMenu();

    if (isAlive) {

      readBrightnessValue();

      readJoystickValues();
      checkDirection();

      movePlayer();

      checkButtonPress();


      // Serial.println(bombsNumber);
      for (int index = 0; index <= maximumBombs; index++) {
        if (bombs[index].isDetonated == false && bombs[index].bombCoutdownStart != 0) {
          bombs[index].checkBombState();
        }
      }

     
      checkTimePass();
      checkLevelRules();

      lightUpMatrix();
      blinkPlayer();
    } else {
      displaySadFace();
      checkButtonPress();
      checkEepromScore();
    }
  }

  //we are navigating through the menu and we haven't pressed the Start Game option yet
  else {
    navigateMenu();
  }
}
