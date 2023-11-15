// Define connections to the shift register
const int latchPin = 11;  // Connects to STCP
const int clockPin = 10;  // Connects to SHCP
const int dataPin = 12;   // Connects to DS

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int lapButton = 2;
const int resetButton = 8;
const int startButton = 3;

volatile bool isLapButtonPressed = false;
volatile bool isStartButtonPressed = false;
volatile bool isResetButtonPressed = false;

volatile bool isCounting = false;
volatile bool isPaused = false;

// Timestamp for when the last interrupt occurred, used for debouncing
volatile unsigned long lastLapInterruptTime = 0;
volatile unsigned long lastStartInterruptTime = 0;
volatile unsigned long lastResetInterruptTime = 0;

const unsigned long debounceDelay = 200;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4;

const int encodingsNumber = 11;

byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B00000001   // DP
};


const int encodingsLetters = 17;

byte letterEncodings[] = {
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110,  // F
  B11100110,  // G
  B00101110,  // H
  B01100000,  // I
  B01110000,  // J
  B00000000,  // (blank)
  B00011100,  // L
  B00110110,  // M
  B11101100,  // N
  B11111100,  // O
  B11001110,  // P
  B10101100,  // q
  B10000100,  // r
  B10011010,  // S
  B00011110,  // t
  B01111100,  // U
  B01110100,  // v
  B11010100,  // W
  B11011110,  // X
  B11011010,  // Y
  B10110110,  // Z
  B00000000,  // (blank)

};

// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 50;  // Delay between updates (milliseconds)
unsigned long number = 0;       // The number being displayed
unsigned long start = 0;       

bool isLapping = false;

byte resetButtonState = LOW;
byte resetReading = HIGH;
unsigned long lastResetDebounceTime = 0;
unsigned long lastResetReading = 0;

int noOfSavedValues = 4;
unsigned long savedValues[] = { 0, 0, 0, 0 };
int currentSaveValue = 0;
int currentIndex = 0;

char text[] = "null";
char longText[] = "hellooo";

int changeTime = 1000;
unsigned long lastChange = 0;
int index = 0;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(lapButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(lapButton), handleLapInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(startButton), handleStartInterrupt, FALLING);
}

void loop() {

  checkStartPausePressed();

  if (isCounting == true) {
    incrementNumber();
  }

  checkLapButtonPressed();

  checkResetButtonPressed();

  if (number == 0 && isLapping) {
    writeLetter(text);
  } else {
    writeNumber(number);
  }

  //scrollText(longText);
}


void checkStartPausePressed() {

  if (isStartButtonPressed) {

    isLapping = false;
    if (isCounting == true) {  //if it is already counting we need to pause it
      isCounting = false;
      isPaused = true;
    }

    else {
      //we reset all the values in our array after a possible reset 
      for (int i = 0; i < noOfSavedValues; i++) {
        savedValues[i] = 0;
      }

      isCounting = true;
      isPaused = false;
    }

    isStartButtonPressed = false;
  }
}

void checkLapButtonPressed() {

  if (isLapButtonPressed) {

    //the button was reset therefore pause and counting are set to false
    //we iterate through the saved values 
    if (!isCounting && !isPaused) {
      currentIndex %= 4;
      isLapping = true;
      number = savedValues[currentIndex];
      currentIndex++;

    }

    //The counter is working and we save the time value when the button is pressed
    else if (isCounting) {
      savedValues[currentSaveValue] = number;
      currentSaveValue++;
      currentSaveValue %= 4;
    }

    isLapButtonPressed = false;
  }
}

void checkResetButtonPressed() {

  //Debounce 
  resetReading = digitalRead(resetButton);
  if (resetReading != lastResetReading) {
    lastResetDebounceTime = millis();
  }
  if ((millis() - lastResetDebounceTime) > debounceDelay) {
    if (resetReading != resetButtonState) {
      resetButtonState = resetReading;
      if (resetButtonState == LOW) {
        isResetButtonPressed = true;
      }
    }
  }

  lastResetReading = resetReading;



  //we reset the timer only if the button is also paused otherwise we do nothing
  if (isResetButtonPressed) {
    if (isPaused) {
      number=start; 
      currentIndex = 0;
      currentSaveValue = 0;
      isResetButtonPressed = false;
      isPaused = false;
      isCounting = false;

    } else {
      isResetButtonPressed = false;
    }
  }
}



void incrementNumber() {
  if (millis() - lastIncrement > delayCount) {
    //number++;

    // if (number > 0) {
    //   number--;
    // }
    lastIncrement = millis();
  }

  if (number > 9999) {
    number = 0;
  }
}

void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeLetter(char text[]) {
  int currentDisplay = 3;
  char lastLetter;

  int length = strlen(text);
  while (length) {
    lastLetter = text[length - 1];
    activateDisplay(currentDisplay);
    writeReg(letterEncodings[lastLetter - 'a']);
    delay(0);
    currentDisplay--;
    length--;
    writeReg(00000000);
  }
}

void scrollText(char text[]) {
  int currentDisplay;
  char currentLetter;
  int length = strlen(text);
  int arrayLength = sizeof(letterEncodings) / sizeof(letterEncodings[0]);

  if (index <= length - 1) {
    if (millis() - lastChange > changeTime) {
      lastChange = millis();
      index++;
    }
    currentDisplay = 0;
    while (currentDisplay <= 3) {
      if (index + currentDisplay > length - 1) {
        activateDisplay(currentDisplay);
        writeReg(letterEncodings[arrayLength - 1]);
      } else {
        currentLetter = text[index + currentDisplay];
        activateDisplay(currentDisplay);
        writeReg(letterEncodings[currentLetter - 'a']);
      }

      currentDisplay++;
      writeReg(00000000);
    }

  }

  else {
    index = 0;
  }
}

void writeNumber(int number) {
  int currentDisplay = 3;
  int noOfDigits = 0;
  int lastDigit = 0;
  int currentNumber = number;

  //We activate the DP on the second display
  activateDisplay(2);
  writeReg(byteEncodings[encodingsNumber-1]);
  writeReg(00000000);

  //we display 0 at the beginning
  if (number == 0) {
    activateDisplay(currentDisplay);
    writeReg(byteEncodings[0]);
    writeReg(00000000);
  }

  else {
    //We take each digit one by one and display it
    while (currentNumber) {
      noOfDigits++;
      lastDigit = currentNumber % 10;
      activateDisplay(currentDisplay);
      writeReg(byteEncodings[lastDigit]);
      currentDisplay--;
      currentNumber /= 10;
      writeReg(00000000);
    }
  }

  //We complete the number with zeros if it doesn't have 4 digits
  if (noOfDigits != 4) {
    for (int i = 0; i < 4 - noOfDigits; i++) {
      activateDisplay(i);
      writeReg(byteEncodings[0]);
      writeReg(00000000);
    }
  }
}


void handleLapInterrupt() {
  static unsigned long interruptTime = 0;  // Timestamp for the current interrupt, retains its value between ISR calls
  interruptTime = micros();

  if (interruptTime - lastLapInterruptTime > debounceDelay * 1000) {
    isLapButtonPressed = true;
  }

  // Update the last interrupt timestamp
  lastLapInterruptTime = interruptTime;
}


void handleStartInterrupt() {
  static unsigned long interruptTime = 0;  // Timestamp for the current interrupt, retains its value between ISR calls
  interruptTime = micros();


  if (interruptTime - lastStartInterruptTime > debounceDelay * 1000) {
    isStartButtonPressed = true;
    Serial.println("start");
  }
  // Update the last interrupt timestamp
  lastStartInterruptTime = interruptTime;
}
