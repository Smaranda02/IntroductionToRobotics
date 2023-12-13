
void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);                    // clear screen

  pinMode(buttonPin, INPUT_PULLUP);  // activate pull-up resistor on the // push-button pin
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, FALLING);

  //to initialize the random number generator with a fairly random input on subsequent executions of a sketch
  randomSeed(analogRead(0));

  rooms[0].walls = noOfWalls;
  rooms[1].walls = noOfWallsLevel2;
  rooms[2].walls = noOfWallsLevel3;
  rooms[3].walls = noOfWallsLevel3;


  for (int index = 0; index < noOfRooms; index++) {
    currentMatrix = *matrixPointers[index];
    generateWalls(*matrixPointers[index], rooms[index].walls);
  }

  generateEnemy(realNoOfEnemies);

  currentMatrix = matrix1;
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, heart);

  setBrightness(55);
  greeted = false;
  podiumDisplayed = false;
  highscoreDisplayed = false;
  nameIndex = -1;
}
