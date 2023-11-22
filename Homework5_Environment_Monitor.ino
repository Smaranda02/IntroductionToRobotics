#include <avr/pgmspace.h>
#include <EEPROM.h>

const char MENU[6][50] PROGMEM = {
  "Menu",
  "1. Sensor Settings",
  "2. Reset Logger Data",
  "3. System Status",
  "4. RGB LED Control",
  ""
};

const char SENSOR_SETTINGS_MENU[6][50] PROGMEM = {
  "Sensor Settings",
  "1. Sensors Sampling Interval",
  "2. Ultrasonic Alert Threshold",
  "3. LDR Alert Threshold",
  "4. Infrared Alert Threshold",
  "5. Back"
};



const char RESET_CONFIRMATION_MENU[6][50] PROGMEM = {
  "Are you sure you want to reset the logger?",
  "1. YES",
  "2. NO",
  "",
  "",
  ""
};

const char SYSTEM_STATUS_MENU[6][50] PROGMEM = {
  "System Status",
  "1. Current Sensor Readings",
  "2. Current Sensor Settings",
  "3. Display Logged Data",
  "4. Back",
  ""
};

const char RGB_LED_CONTROL_MENU[6][50] PROGMEM = {
  "RGB LED Control",
  "1. Manual Color Control",
  "2. LED: Toggle Automatic ON/OFF",
  "3. Back",
  "",
  ""
};


const char *const menuTable[][6] PROGMEM = {
  { MENU[0], MENU[1], MENU[2], MENU[3], MENU[4], MENU[5] },
  { SENSOR_SETTINGS_MENU[0], SENSOR_SETTINGS_MENU[1], SENSOR_SETTINGS_MENU[2], SENSOR_SETTINGS_MENU[3], SENSOR_SETTINGS_MENU[4], SENSOR_SETTINGS_MENU[5] },
  { RESET_CONFIRMATION_MENU[0], RESET_CONFIRMATION_MENU[1], RESET_CONFIRMATION_MENU[2], RESET_CONFIRMATION_MENU[3], RESET_CONFIRMATION_MENU[4], RESET_CONFIRMATION_MENU[5] },
  { SYSTEM_STATUS_MENU[0], SYSTEM_STATUS_MENU[1], SYSTEM_STATUS_MENU[2], SYSTEM_STATUS_MENU[3], SYSTEM_STATUS_MENU[4], SYSTEM_STATUS_MENU[5] },
  { RGB_LED_CONTROL_MENU[0], RGB_LED_CONTROL_MENU[1], RGB_LED_CONTROL_MENU[2], RGB_LED_CONTROL_MENU[3], RGB_LED_CONTROL_MENU[4], RGB_LED_CONTROL_MENU[5] }
};

int currentMenu = 0;
int currentOption = 0;


char buffer[50];


//variables
int samplingRate = 1;
int ultrasonicMaxThreshold = 20;
int minLDRThreshold = 100;  //signaling night what is under 100
int maxLDRThreshold = 300;

int maxIrThreshold = 20;
bool shown = false;

int userInput = 0;

unsigned long lastLdrReading = 0;
unsigned long lastUltrasonicReading = 0;


//variables for LED and LDR
const int redLedPin = 5;
const int greenLedPin = 3;

int redLedState = 0;
int greenLedState = 255;


int photocellPin = A0;
int photocellValue;


volatile bool automaticLedState = true;
volatile bool ldrThresholdExceeded = false;
volatile bool ultrasonicThresholdExceeded = false;
volatile bool irThresholdExceeded = false;


//ultrasonic
const int trigPin = 11;
const int echoPin = 10;
long duration = 0;
int distance = 0;

//infrared
const int irSensorPin = A5;
float irSensorvalue = 0;
float volts = 0.0;
int distanceIR = 0;
unsigned long lastIrReading;

//arrays
const int maxLoggedData = 10;

int loggedUltrasonicData[maxLoggedData];
int loggedLdrData[maxLoggedData];
int loggedIrData[maxLoggedData];

int ultrasonicDataIndex = maxLoggedData - 1;
int ldrDataIndex = maxLoggedData - 1;
int irDataIndex = maxLoggedData - 1;

//buzzer
const int buzzerPin = 7;
int buzzerTone = 3000;
volatile bool buzzed = false;

void setup() {
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  //for ultrasonic
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);

  EEPROM.update(0, ultrasonicMaxThreshold);
  EEPROM.update(1, minLDRThreshold);
  EEPROM.update(2, maxIrThreshold);
  EEPROM.update(3, 255);
}

void loop() {

  checkSensors();

  //we check on which menu and which option (submenu) we are currently on
  switch (currentMenu) {
    case 0:
      menu0Options();
      break;
    case 1:
      menu1Options(currentOption);
      break;
    case 2:
      menu2Options(currentOption);
      break;
    case 3:
      menu3Options(currentOption);
      break;
    case 4:
      menu4Options(currentOption);
      break;
  }
}

void checkSensors() {
  //we constantly read values from sensors and store them if needed
  ultrasonicValues();
  LDRValues();
  //infraRedValues();
  displayLedState();
}

void displayLedState() {

  int ledIntensity = EEPROM.read(3);

  //if any of the thresholds are exceeded we turn the red LED ON
  if (ultrasonicThresholdExceeded || ldrThresholdExceeded || irThresholdExceeded) {
    greenLedState = 0;

  } else {
    greenLedState = ledIntensity;
  }

  redLedState = (greenLedState == ledIntensity ? 0 : ledIntensity);


  //if the red LED is ON we activate the buzzer
  if (redLedState == 0) {
    buzzed = false;
  } else {
    if (!buzzed) {
      tone(buzzerPin, buzzerTone, 1000);
      buzzed = true;
    }
  }

  analogWrite(greenLedPin, greenLedState);
  analogWrite(redLedPin, redLedState);
}

void menu0Options() {
  currentMenu = 0;

  //we make sure we display the menu only one time
  if (!shown) {
    displayMenu(0);
    Serial.println(F("Enter your option"));
    shown = true;
  }


  if (Serial.available() > 0) {
    int input = Serial.parseInt();


    //before we go to a submenu we reset the shown variable to false in order to display the menu
    switch (input) {
      case 1:
        {
          shown = false;
          menu1Options(0);
        }
        break;
      case 2:
        {
          shown = false;
          menu2Options(0);
        }

        break;
      case 3:
        {
          shown = false;
          menu3Options(0);
        }
        break;
      case 4:
        {
          shown = false;
          menu4Options(0);
        }
        break;
      default:
        {
          Serial.println(F("Invalid option"));
        }
    }
  }
}


void menu1Options(int option) {

  currentMenu = 1;
  if (!shown) {
    displayMenu(1);
    Serial.println(F("Enter your option"));
    shown = true;
  }

  if (Serial.available() > 0) {

    if (currentOption == 0) {
      currentOption = Serial.parseInt();
      option = currentOption;
    }


    switch (option) {

      case 0:
        menu1Options(0);
        break;
      case 1:
        {
          shown = false;
          currentOption = 1;
          sensorSamplingInput();
        }
        break;
      case 2:
        {
          shown = false;
          currentOption = 2;
          ultrasonicThreshold();
        }
        break;
      case 3:
        {
          shown = false;
          currentOption = 3;
          ldrThreshold();
        }
        break;
      case 4:
        {
          shown = false;
          currentOption = 4;
          infraredThreshold();
        }
        break;
      case 5:
        {
          shown = false;
          currentOption = 0;
          menu0Options();
        }
        break;
      default:
        {
          currentOption = 0;
          Serial.println(F("Invalid option"));
        }
    }
  }
}


void ldrThreshold() {
  if (!shown) {
    Serial.print(F("Enter the min LDR threshold between "));
    Serial.print(minLDRThreshold);
    Serial.print(" and ");
    Serial.println(maxLDRThreshold);
    shown = true;
  }
  if (Serial.available() > 0) {
    int input = Serial.parseInt();

    //if the read value is between our parameters we store it to EEPROM
    if (input <= maxLDRThreshold && input >= minLDRThreshold) {
      //minLDRThreshold = input;
      EEPROM.update(1, input);
      shown = false;
      currentOption = 0;
      Serial.print(F("You chose the min threshold for LDR of : "));
      Serial.println(input);

    } else {
      Serial.print(F("The min threshold for LDR of : "));
      Serial.print(input);
      Serial.println(F("is out of bounds"));
    }
  }
}

void ultrasonicThreshold() {

  if (!shown) {
    Serial.print(F("Enter the Maximum Ultrasonic Alert Threshold under "));
    Serial.println(ultrasonicMaxThreshold);
    shown = true;
  }
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    if (input <= ultrasonicMaxThreshold) {
      EEPROM.update(0, input);
      shown = false;
      currentOption = 0;
      Serial.print(F("You chose the maximum threshold of : "));
      Serial.println(input);

    } else {
      Serial.print(F("The maximum threshold of : "));
      Serial.print(input);
      Serial.println(F("is too high"));
    }
  }
}


void infraredThreshold() {

  //for threshold
  if (!shown) {
    Serial.print(F("Enter the Maximum Infrared Alert Threshold under "));
    Serial.println(maxIrThreshold);

    shown = true;
  }
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    if (input <= maxIrThreshold) {
      EEPROM.update(0, input);
      shown = false;
      currentOption = 0;
      Serial.print(F("You chose the maximum threshold of : "));
      Serial.println(input);

    } else {
      Serial.print(F("The maximum threshold of : "));
      Serial.print(input);
      Serial.println(F("is too high"));
    }
  }
}


void sensorSamplingInput() {

  if (!shown) {
    Serial.println(F("Enter the sampling rate between 1 and 10 "));
    shown = true;
  }
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    if (input >= 1 && input <= 10) {
      samplingRate = input;
      shown = false;
      currentOption = 0;
      Serial.print(F("You chose the sampling rate of : "));
      Serial.println(samplingRate);

    } else {
      Serial.print(F("The sampling rate "));
      //Serial.print(samplingRate);
      Serial.println(F("is out of bounds"));
    }
  }
}

void menu2Options(int option) {
  currentMenu = 2;
  if (!shown) {
    displayMenu(2);
    Serial.println(F("Enter your option"));
    shown = true;
  }


  if (Serial.available() > 0) {

    if (currentOption == 0) {
      currentOption = Serial.parseInt();
      option = currentOption;
    }
  }

  switch (currentOption) {

    case 0:
      menu2Options(0);
      break;
    case 1:
      {
        currentOption = 1;
        resetLoggedData();
      }
      break;
    case 2:
      {
        shown = false;
        currentOption = 0;
        menu0Options();
      }
      break;
    default:
      {
        currentOption = 0;
        Serial.println(F("Invalid option"));
      }
  }
}

void resetLoggedData() {
  for (int i = 0; i < maxLoggedData; i++) {
    loggedUltrasonicData[i] = 0;
    loggedLdrData[i] = 0;
  }

  currentMenu = 0;
  currentOption = 0;
  shown = false;
}


void menu3Options(int option) {

  currentMenu = 3;
  if (!shown) {
    displayMenu(3);
    Serial.println(F("Enter your option"));
    shown = true;
  }

  if (Serial.available() > 0) {

    if (currentOption == 0) {
      currentOption = Serial.parseInt();
      option = currentOption;
    }
  }

  switch (currentOption) {

    case 0:
      menu3Options(0);
      break;
    case 1:
      {
        currentOption = 1;
        currentSensorReadings();
      }
      break;
    case 2:
      {
        currentOption = 2;
        currentSensorSettings();
      }
      break;
    case 3:
      {
        currentOption = 3;
        displayLoggedData();
      }
      break;
    case 4:
      {
        shown = false;
        currentOption = 0;
        menu0Options();
      }
      break;
    default:
      {
        currentOption = 0;
        Serial.println(F("Invalid option"));
      }
  }
}


void currentSensorReadings() {

  char input;
  if (Serial.available() > 0) {
    input = (char)Serial.read();
  }

  //we stop reading when the user presses 'x'
  if (input != 'x') {
    LDRValues();
    ultrasonicValues();
    //infraRedValues();

  } else {
    shown = false;
    currentOption = 0;
  }
}


void currentSensorSettings() {

  Serial.println("---------------");

  Serial.print(F("The threshold for the ultrasonic sensor is : "));
  Serial.println(EEPROM.read(0));

  Serial.print(F("The threshold for the LDR sensor is : "));
  Serial.println(EEPROM.read(1));

  Serial.print(F("The threshold for the infrared sensor is : "));
  Serial.println(EEPROM.read(2));

  currentOption = 0;
  shown = false;
}

void displayLoggedData() {
  Serial.println(F("The last 10 ultrasonic sensor readings are : "));
  for (int i = 0; i < maxLoggedData; i++) {
    displayLoggedDataFormat(loggedUltrasonicData[i], i);
  }

  Serial.println("---------------");


  Serial.println(F("The last 10 LDR sensor readings are : "));
  for (int i = 0; i < maxLoggedData; i++) {
    displayLoggedDataFormat(loggedLdrData[i], i);
  }

  Serial.println("---------------");


  Serial.println(F("The last 10 IR sensor readings are : "));
  for (int i = 0; i < maxLoggedData; i++) {
    displayLoggedDataFormat(loggedIrData[i], i);
  }

  currentOption = 0;
  shown = 0;
}

void displayLoggedDataFormat(int value, int index) {
  Serial.print(F("Value "));
  Serial.print(index);
  Serial.print(" : ");
  Serial.println(value);
}


void LDRValues() {

  if (millis() - lastLdrReading >= samplingRate * 1000) {

    readLDR();

    //print and log LDR data
    if (currentMenu == 3 && currentOption == 1) {
      printLDRValues();
      logLDRValues();
    }

    checkLdrThreshold();
    lastLdrReading = millis();
  }
}

void logLDRValues() {
  loggedLdrData[ldrDataIndex] = photocellValue;
  ldrDataIndex++;
  if (ldrDataIndex > 9) {
    ldrDataIndex = 0;
  }
}

void printLDRValues() {
  Serial.println(F("----------------"));
  Serial.print(F("LDR reading = "));
  Serial.println(photocellValue);  // the raw analog reading
}

void readLDR() {
  photocellValue = analogRead(photocellPin);
}


void checkLdrThreshold() {

  if (automaticLedState) {
    if (photocellValue < EEPROM.read(1)) {
      ldrThresholdExceeded = true;
    } else {
      ldrThresholdExceeded = false;
    }
  }
}

void infraRedValues() {

  if (millis() - lastIrReading >= samplingRate) {

    float irSensorValue = analogRead(irSensorPin);
    float voltage = irSensorValue * 5 / 1024;  // switching from 0..1023 to 0..5V
    distanceIR = 13 * pow(voltage, -1);        // extracted from the datasheet

    if (currentMenu == 3 && currentOption == 1) {
      printInfraredValues();
      logInfraredValues();
    }

    checkIrThreshold();
    lastIrReading = millis();
  }
}

void printInfraredValues() {
  Serial.print(F("Distance Infrared: "));
  Serial.println(distanceIR);

  Serial.println(F("Press 'x' to exit the leave logger"));
  Serial.println(F("----------------"));
}

void logInfraredValues() {
  loggedIrData[irDataIndex] = distanceIR;
  irDataIndex++;
  if (irDataIndex > 9) {
    irDataIndex = 0;
  }
}

void checkIrThreshold() {

  if (automaticLedState) {
    if (distanceIR < EEPROM.read(2)) {
      irThresholdExceeded = true;
    } else {
      irThresholdExceeded = false;
    }
  }
}

void ultrasonicValues() {


  if (millis() - lastUltrasonicReading >= samplingRate * 1000) {

    readUltrasonic();

    //print and log data
    if (currentMenu == 3 && currentOption == 1) {
      printUltrasonicData();
      logUltrasonicData();
    }


    //always check the threshold is not exceeded
    checkUltrasonicThreshold();
    lastUltrasonicReading = millis();
  }
}


void printUltrasonicData() {
  Serial.print(F("Distance: "));
  Serial.println(distance);

  Serial.println(F("Press 'x' to exit the live logger"));
  Serial.println(F("----------------"));
}

void logUltrasonicData() {
  loggedUltrasonicData[ultrasonicDataIndex] = distance;
  ultrasonicDataIndex++;
  if (ultrasonicDataIndex > 9) {
    ultrasonicDataIndex = 0;
  }
}


void readUltrasonic() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we consider half of the distance traveled.
  distance = duration * 0.034 / 2;
}

void checkUltrasonicThreshold() {

  if (automaticLedState) {
    if (distance < EEPROM.read(0)) {
      ultrasonicThresholdExceeded = true;
    } else {
      ultrasonicThresholdExceeded = false;
    }
  }
}



void menu4Options(int option) {
  currentMenu = 4;
  if (!shown) {
    displayMenu(4);
    Serial.println(F("Enter your option"));
    shown = true;
  }

  if (Serial.available() > 0) {

    if (currentOption == 0) {
      currentOption = Serial.parseInt();
      option = currentOption;
    }

    switch (option) {

      case 0:
        menu4Options(0);
        break;
      case 1:
        {
          shown = false;
          currentOption = 1;
          manualColorControl();
        }
        break;
      case 2:
        {
          toggleAutomaticLedState();
        }
        break;
      case 3:
        {
          shown = false;
          currentOption = 0;
          menu0Options();
        }
        break;
      default:
        {
          currentOption = 0;
          Serial.println(F("Invalid option"));
        }
    }
  }
}


void manualColorControl() {

  if (!shown) {
    Serial.println(F("Enter a value between 0 and 255 to edit the intentisy of the LEDs"));
    shown = true;
  }
  if (Serial.available() > 0) {
    int input = Serial.parseInt();

    //here we let the user to manually control the intensity of the LEDs
    if (input >= 0 && input <= 255) {
      EEPROM.update(3, input);
      shown = false;
      currentOption = 0;
      Serial.print(F("You chose  : "));
      Serial.println(input);

    } else {
      Serial.print(F("The intensity "));
      Serial.println(F("is out of bounds"));
    }
  }
}



void toggleAutomaticLedState() {
  Serial.print(F("Automatic led state is now "));
  automaticLedState = !automaticLedState;
  if (automaticLedState) {
    Serial.println(F("ON"));
  } else {
    Serial.println(F("OFF"));
  }

  shown = false;
  currentOption = 0;
}


void displayMenu(int option) {


  Serial.println(F("---------------"));
  for (int j = 0; j < 6; j++) {
    //read each line from the menu from Flash memory and load it into the buffer
    strcpy_P(buffer, (char *)pgm_read_ptr(&(menuTable[option][j])));
    Serial.println(buffer);
  }
  Serial.println(F("---------------"));
}
