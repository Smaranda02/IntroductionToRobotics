void navigateMenu() {
  //when we first power up the game
  if (startGreet == 0) {
    startGreet = millis();
  }

  if (!greeted) {
    if (millis() - startGreet >= greetingDisplayTime) {
      inMenu = true;
      lcd.clear();
      greeted = true;
    } else {
      displayGreeting();
    }
  }

  else {
    if (inMenu) {
      checkMenuOption();
    } else if(inGame) {
      displayGameDetails();
    }
  }
}

void displayGreeting() {
  lcd.setCursor(0, 0);
  lcd.print(greetingMessage.substring(0, 16));

  // Move to the second line
  lcd.setCursor(0, 1);

  // Display the remaining characters
  lcd.print(greetingMessage.substring(16));
}


void checkMenuOption() {

  switch (currentMenu) {
    case MainMenu:
      mainMenu();
      break;
    case StartMenu:
      StartGameMenu();
      break;
    case AboutGameMenu:
      AboutMenu();
      break;
    case HighscoreMenu:
      HighscoresMenu();
      break;
    case GameSettingsMenu:
      SettingsMenu();
      break;
    case InstructionsMenu:
      HowToPlayMenu();
      break;
    case GameRestartMenu:
      checkGameRestart();
      break;
    case BrightnessControlMenu:
      brightnessControl();
      break;
    case EnterNameMenu:
      enterNameMenu();
      break;
    case SaveNameMenu:
      saveNameMenu();
      break;
    case SoundMenu:
      soundMenu();
      break;
  }
}


void mainMenu() {
  currentMenu = MainMenu;

  if (!shown) {
    lcd.clear();
    shown = true;
  }

  //we update if needed the currentOption
  menuSize = mainMenuSize;
  menuPointer = MENU;
  checkSelection();
  displayMenu();

  //only if we confirm our option by pressing the button we go to that option
  if (isButtonPressed) {

    isButtonPressed = false;

    switch (currentOption) {
      case 0:
        {
          shown = false;
          currentOption = 0;
          StartGameMenu();
        }
        break;
      case 1:
        {
          shown = false;
          currentOption = 0;
          AboutMenu();
        }
        break;

      case 2:
        {
          shown = false;
          currentOption = 0;
          HighscoresMenu();
        }

        break;

      case 3:
        {
          shown = false;
          currentOption = 0;
          SettingsMenu();
        }

        break;

      case 4:
        {
          shown = false;
          currentOption = 0;
          HowToPlayMenu();
        }

        break;
    }
  }
}


void StartGameMenu() {

  currentMenu = StartMenu;
  if (!shown) {
    lcd.clear();
    shown = true;
  }

  menuSize = startGameMenuSize;
  checkSelection();
  printArrow(1, 0, 1, 9);
  displayStartGame();


  if (isButtonPressed) {

    isButtonPressed = false;

    switch (currentOption) {

      case 0:
        {
          shown = false;
          gameStarted = true;
          inGame = true;
          displayGameDetails();
        }
        break;
      case 1:
        {
          shown = false;
          currentOption = 0;
          mainMenu();
        }
        break;
    }
  }
}


void AboutMenu() {
  currentMenu = AboutGameMenu;

  if (!shown) {
    lcd.clear();
    shown = true;
  }


  menuSize = aboutMenuSize;
  checkSelection();
  menuPointer = About;
  displayMenu();

  if (isButtonPressed && currentOption == menuSize - 1) {
    isButtonPressed = false;
    currentMenu = 0;
    currentOption = 0;
    shown = false;
  }
}


void scrollMessages(const char* message) {
  byte length = strlen(message);
  byte line;
  byte column;

  if (currentOption % 2 == 0) {
    column = 1;
    line = 0;
  } else {
    line = 1;
    column = 1;
  }


  if (scrollIndex <= length) {
    lcd.setCursor(column, line);

    lcd.print(message + scrollIndex);
    if (strlen(message + scrollIndex) < 16) {
      clearLCD(message + scrollIndex);
    }

    if (millis() - lastScroll >= scrollTime) {
      scrollIndex++;
      lastScroll = millis();
    }
  }

  else {
    scrollIndex = 0;
  }
}

void checkSelection() {

  readJoystickValues();
  checkDirection();

  switch (currentMove) {
    //UP
    case UP:
      {
        if (currentOption > 0) {
          currentOption--;
          currentMove = -1;
        }
      }
      break;

    //DOWN
    case DOWN:
      {
        if (currentOption < menuSize - 1) {
          currentOption++;
          currentMove = -1;
        }
      }
      break;
  }
}

void displayMenu() {

  byte first;
  byte second;

  char buffer[30];

  if (currentOption % 2 == 0) {
    first = currentOption;
    second = currentOption + 1;

    //if (strlen(menuPointer[currentOption]) > 16) {
    strcpy_P(buffer, (char*)pgm_read_word(&menuPointer[currentOption]));

    if (strlen(buffer) > 16) {

      if (scrollOnFirst == false) {
        //at a change of scroll element we reset the scroll Index
        scrollIndex = 0;
      }
      scrollOnFirst = true;
    } else {
      scrollOnFirst = false;
    }
    scrollOnSecond = false;


  }

  else {
    first = currentOption - 1;
    second = currentOption;

    //if (strlen(menuPointer[currentOption]) > 16) {
    strcpy_P(buffer, (char*)pgm_read_word(&menuPointer[currentOption]));

    if (strlen(buffer) > 16) {
      if (scrollOnSecond == false) {
        scrollIndex = 0;
      }
      scrollOnSecond = true;
    } else {
      scrollOnSecond = false;
    }
    scrollOnFirst = false;
  }

  printArrow(0, 0, 1, 0);

  char bufferFirst[30];
  char bufferSecond[30];
  //lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&menuPointer[second])));  // Read PROGMEM pointers
  strcpy_P(bufferFirst, (char*)pgm_read_word(&menuPointer[first]));
  if (second <= menuSize - 1) {
    strcpy_P(bufferSecond, (char*)pgm_read_word(&menuPointer[second]));
  }

  if (!scrollOnFirst) {
    lcd.setCursor(1, 0);
    lcd.print(bufferFirst);
    //we clear the artifacts from the last printings on the display (About appears Aboutings)
    clearLCD(bufferFirst);
  } else {
    scrollMessages(bufferFirst);
  }

  if (!scrollOnSecond) {
    if (second <= menuSize - 1) {
      lcd.setCursor(1, 1);
      lcd.print(bufferSecond);
      clearLCD(bufferSecond);
    } else {
      lcd.setCursor(1, 1);
      lcd.print(F("                "));
    }
  }

  else {
    scrollMessages(bufferSecond);
  }
}


void clearLCD(char* message) {
  byte length = strlen(message);
  for (byte i = length; i < 16; i++) {
    lcd.print(" ");
  }
}


void displayStartGame() {
  lcd.setCursor(0, 0);
  lcd.print(StartGameMessage);
  lcd.setCursor(0, 1);
  lcd.print(arrow);

  lcd.setCursor(1, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&StartGame[0])));  // Read PROGMEM pointers
  lcd.setCursor(10, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&StartGame[1])));  // Read PROGMEM pointers
}

void printArrow(byte line1, byte column1, byte line2, byte column2) {
  if (currentOption % 2 == 0) {
    lcd.setCursor(column2, line2);
    lcd.write(' ');
    lcd.setCursor(column1, line1);

  } else {
    lcd.setCursor(column1, line1);
    lcd.write(' ');
    lcd.setCursor(column2, line2);
  }
  lcd.print(arrow);
}


void displayGameDetails() {

  if (!shown) {
    lcd.clear();
    resetGame();
    shown = true;
  }

  inMenu = false;

  if (!anotherMessage) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));  // Display the custom character from the specified index in the special characters array
    lcd.setCursor(1, 0);
    lcd.print(lives);

    lcd.setCursor(5, 0);
    lcd.print(F("Walls:"));
    lcd.setCursor(11, 0);
    lcd.print(wallsCoverage);

    lcd.setCursor(0, 1);
    lcd.print(F("Points:"));
    lcd.setCursor(7, 1);
    lcd.print(score);

    lcd.setCursor(9, 1);
    lcd.print(F("Time:"));
    lcd.setCursor(14, 1);
    lcd.print(time);
  }
}

void HighscoresMenu() {
  currentMenu = HighscoreMenu;

  menuSize = 4;
  checkSelection();
  printArrow(0, 0, 1, 0);
  displayHighscores();


  if (isButtonPressed && currentOption == menuSize - 1) {
    isButtonPressed = false;
    currentMenu = 0;
    currentOption = 0;
    shown = false;
  }
}

void displayHighscores() {

  char firstName[maxNameCharacters] = { EEPROM.read(firstPlayerMemoryLocation) };
  char secondName[maxNameCharacters] = { EEPROM.read(secondPlayerMemoryLocation) };
  char thirdName[maxNameCharacters] = { EEPROM.read(thirdPlayerMemoryLocation) };

  byte first = EEPROM.read(firstPlayerMemoryLocation + maxNameCharacters);
  byte second = EEPROM.read(secondPlayerMemoryLocation + maxNameCharacters);
  byte third = EEPROM.read(thirdPlayerMemoryLocation + maxNameCharacters);

  String firstPlayer = "#1 " + String(firstName) + " " + String(first);
  String secondPlayer = "#2 " + String(secondName) + " " + String(second);
  String thirdPlayer = "#3 " + String(thirdName) + " " + String(third);

  char Back[4] = "Back";

  if (!shown) {
    lcd.clear();
    shown = true;
  }

  if (currentOption % 2 == 0) {
    first = currentOption;
    second = currentOption + 1;
  }

  else {
    first = currentOption - 1;
    second = currentOption;
  }

  if (first == 0) {
    lcd.setCursor(1, 0);
    lcd.print(firstPlayer);
    clearLCDString(firstPlayer);
    lcd.setCursor(1, 1);
    lcd.print(secondPlayer);
    clearLCDString(secondPlayer);

  } else {
    lcd.setCursor(1, 0);
    lcd.print(thirdPlayer);
    clearLCDString(thirdPlayer);
    lcd.setCursor(1, 1);
    lcd.print(Back);
    clearLCDString(Back);
  }
}

void clearLCDString(String string) {
  for (int index = string.length() + 1; index < 16; index++) {
    lcd.print(' ');
  }
}

void checkGameRestart() {

  currentMenu = GameRestartMenu;
  if (!shown) {
    shown = true;
    lcd.clear();
  }


  menuSize = restartGameMenuSize;
  checkSelection();
  printArrow(1, 0, 1, 9);
  displayRestartGame();


  if (isButtonPressed) {

    isButtonPressed = false;

    switch (currentOption) {

      case 0:
        {
          shown = false;
          gameStarted = true;
          displayGameDetails();
        }
        break;
      case 1:
        {
          shown = false;
          currentOption = 0;
          mainMenu();
        }
        break;
    }
  }
}


void displayRestartGame() {
  lcd.setCursor(0, 0);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(gameOverMessage));

  lcd.setCursor(1, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&RestartGame[0])));

  lcd.setCursor(10, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&RestartGame[1])));
}


void SettingsMenu() {
  currentMenu = GameSettingsMenu;
  if (!shown) {
    lcd.clear();
    shown = true;
  }

  menuSize = settingsMenuSize;
  menuPointer = Settings;
  checkSelection();
  displayMenu();

  if (isButtonPressed) {

    isButtonPressed = false;

    switch (currentOption) {

      case 0:
        {
          shown = false;
          currentOption = 0;
          controlVariable = 0;
          brightnessControl();
        }
        break;
      case 1:
        {
          shown = false;
          currentOption = 0;
          controlVariable = 1;
          brightnessControl();
        }
        break;
      case 2:
        {
          shown = false;
          currentLetter = 0;
          enterNameMenu();
        }
        break;
      case 3:
        {
          shown = false;
          currentOption = 0;
          soundMenu();
        }
        break;
      case 4:
        {
          shown = false;
          currentOption = 0;
          turnOffMatrix();
          mainMenu();
        }
        break;
    }
  }
}

void soundMenu() {

  currentMenu = SoundMenu;

  if (!shown) {
    lcd.clear();
    shown = true;
  }

  checkSelection();
  printArrow(0, 0, 1, 0);
  displaySoundMenu();

  if (isButtonPressed) {

    isButtonPressed = false;

    if (currentOption == 0) {
      soundValue = !soundValue;
    }

    else {
      shown = false;
      currentOption = 0;
      SettingsMenu();
    }
  }
}

void displaySoundMenu(){

  lcd.setCursor(1,0);
  lcd.print(F("Sound : "));
  lcd.setCursor(9,0);
  if (soundValue) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }

  lcd.setCursor(1,1);
  lcd.print(F("Back"));
 
}

void enterNameMenu() {
  currentMenu = EnterNameMenu;
  displayAlphabet();

  checkLetterSelection();
  checkCurrentLetter();


  if (isButtonPressed) {

    isButtonPressed = false;

    //any letter
    if (currentLetter >= 0 && currentLetter <= 25) {
      if (nameIndex < maxNameCharacters - 1) {
        nameIndex++;
        name[nameIndex] = currentLetterValue;
      }
    }

    //N
    else if (currentLetter == 31) {
      shown = false;
      currentOption = 0;
      nameIndex = -1;
      for (int index = 0; index < maxNameCharacters; index++) {
        name[index] = ' ';
      }
      SettingsMenu();
    }

    //Y
    else if (currentLetter == 29) {
      shown = false;
      saveNameMenu();
    }

    //<
    else if (currentLetter == 27) {
      name[nameIndex] = ' ';
      if (nameIndex > 0) {
        nameIndex--;
      }
    }
  }
}


void saveNameMenu() {
  currentMenu = SaveNameMenu;
  if (!shown) {
    lcd.clear();
    shown = true;
  }

  menuSize = saveNameMenuSize;
  checkSelection();
  printArrow(1, 0, 1, 9);
  displaySaveName();


  if (isButtonPressed) {

    isButtonPressed = false;

    switch (currentOption) {

      //after save we get back to the menu
      case 0:
        {
          shown = false;
          currentOption = 0;
          mainMenu();
        }
        break;
      case 1:
        {
          shown = false;
          enterNameMenu();
        }
        break;
    }
  }
}

//!!!!
void displaySaveName() {

  lcd.setCursor(0, 0);
  lcd.print(F("Your name: "));
  lcd.setCursor(11, 0);
  for (int index = 0; index < maxNameCharacters; index++) {
    lcd.print(name[index]);
  }

  lcd.setCursor(1, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&EnterName[0])));

  lcd.setCursor(10, 1);
  lcd.print(reinterpret_cast<const __FlashStringHelper*>(pgm_read_word(&EnterName[1])));
}


//we blink the selected letter
void checkCurrentLetter() {

  if (millis() - lastLetterBlink >= letterBlinkTime) {
    letterState = !letterState;
    lastLetterBlink = millis();
  }

  if (currentLetter <= 25) {
    currentLetterValue = char('A' + currentLetter);
  }

  else {
    switch (currentLetter) {
      case 27:
        currentLetterValue = '<';
        break;
      case 29:
        currentLetterValue = 'Y';
        break;
      case 30:
        currentLetterValue = '/';
        break;
      case 31:
        currentLetterValue = 'N';
        break;
      default:
        currentLetterValue = ' ';
        break;
    }
  }

  if (currentLetter >= 0 && currentLetter <= 15) {
    if (letterState) {
      alphabet1[currentLetter] = currentLetterValue;
    } else {
      alphabet1[currentLetter] = ' ';
    }
  }

  else {
    if (letterState) {
      alphabet2[currentLetter % 16] = currentLetterValue;
    } else {
      alphabet2[currentLetter % 16] = ' ';
    }
  }
}


void checkLetterSelection() {

  readJoystickValues();
  checkDirection();

  if (currentMove != -1) {

    switch (currentMove) {
      //UP
      case UP:
        {
          if (currentLetter > 15) {
            changeLetter();
            currentLetter -= 16;
          }
          currentMove = -1;
        }
        break;

      //DOWN
      case DOWN:
        {
          if (currentLetter < 16) {
            changeLetter();
            currentLetter += 16;
          }
          currentMove = -1;
        }
        break;

      //RIGHT
      case RIGHT:
        {
          if (currentLetter % 16 < 15) {
            changeLetter();
            currentLetter++;
          }
          currentMove = -1;
        }
        break;

      //LEFT
      case LEFT:
        {
          if (currentLetter % 16 > 0) {
            changeLetter();
            currentLetter--;
          }
          currentMove = -1;
        }
        break;
    }
  }
}


void changeLetter() {

  if (currentLetter > 15) {
    alphabet2[currentLetter % 16] = currentLetterValue;
  } else {
    alphabet1[currentLetter] = currentLetterValue;
  }
}

void brightnessControl() {

  currentMenu = 7;
  if (!shown) {
    shown = true;
    lcd.clear();
  }

  menuSize = joystickControlMenuSize;
  checkSelection();
  menuPointer = JoystickControl;
  displayMenu();

  if (currentOption == 0) {
    brightnessValues();
  }

  if (isButtonPressed && currentOption == 1) {
    isButtonPressed = false;
    shown = false;
    currentOption = 0;
    SettingsMenu();
  }
}

void brightnessValues() {

  readJoystickValues();
  checkDirection();

  turnOnMatrix();

  switch (currentMove) {
    case RIGHT:
      {
        //lcd
        if (controlVariable == 0) {
          backLightValue += 10;
        } else {
          matrixBrightness += 1;
        }
        currentMove = -1;
      }
      break;

    case LEFT:
      {
        if (controlVariable == 0) {
          backLightValue -= 10;
        } else {
          matrixBrightness -= 1;
        }
        currentMove = -1;
      }
      break;
  }

  setBrightness(backLightValue);
  lc.setIntensity(0, matrixBrightness);
}

void turnOnMatrix() {
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++) {
      lc.setLed(0, i, j, 1);
    }
}


void turnOffMatrix() {
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++) {
      lc.setLed(0, i, j, 0);
    }
}


void HowToPlayMenu() {
  currentMenu = InstructionsMenu;
  if (!shown) {
    lcd.clear();
    shown = true;
  }

  menuSize = howToMenuSize;
  menuPointer = HowTo;
  checkSelection();
  displayMenu();

  if (isButtonPressed && currentOption == menuSize - 1) {
    isButtonPressed = false;
    currentMenu = 0;
    currentOption = 0;
    shown = false;
  }
}
