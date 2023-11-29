
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
  generateWalls();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print(greetingMessage.substring(0, 16));

  // Move to the second line
  lcd.setCursor(0, 1);

  // Display the remaining characters
  lcd.print(greetingMessage.substring(16));
}
