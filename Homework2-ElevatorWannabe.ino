
const int buzzerPin = 10;
int buzzerArrivalTone = 3000;
int buzzerMovingTone = 500;
int buzzerClosingDoorTone = 1000;

unsigned long lastClosingDoors = 0;

const int flickeringLed = 9;
int flickeringLedState = HIGH;

bool arrived = true;
int movingIndicatorInterval = 500;
int lastFlicker = 0;

unsigned int debounceDelay = 50;
unsigned int floorChangeTime = 2000;
unsigned long lastFloorChange = 0;

bool buzzed = false;
bool doorsClosing = false;



//We create a generic structure that encapsulates all the necessary details about a certain floor
struct Floor {
  const int floorButton;
  const int floorLed;
  bool floorLedState = LOW;
  byte buttonState = LOW;
  byte lastButtonState = LOW;
  unsigned long lastDebounceTime = 0;
  bool buttonStateReading = LOW;

  Floor(int buttonPin, int ledPin)
    : floorButton(buttonPin), floorLed(ledPin) {}
};

Floor floors[3] = {
  Floor(2, 11),
  Floor(3, 12),
  Floor(4, 13)
};

int currentFloor = 0;
int destination = 0;
int totalFloors = sizeof(floors) / sizeof(floors[0]);
int closingDoorFloor = 0;

void setup() {
  Serial.begin(9600);

  floors[0].floorLedState = HIGH;

  for (int i = 0; i < totalFloors; i++) {
    pinMode(floors[i].floorButton, INPUT_PULLUP);
    pinMode(floors[i].floorLed, OUTPUT);
    digitalWrite(floors[i].floorLed, floors[i].floorLedState);
  }

  pinMode(flickeringLed, OUTPUT);
  digitalWrite(flickeringLed, flickeringLedState);
}

void loop() {

  readButtonValues();
  checkButtonPress();


  if (currentFloor != destination) {
    arrived = false;

    //The buzzed variable is false as long as the elevator is moving towards its destination
    buzzed = false;

    //We wait for the "doors to close" which translates to the floorChangeTime variable to pass
    if (millis() - lastClosingDoors < floorChangeTime) {
      closingDoor();
    }

    else {
      doorsClosing = false;
      digitalWrite(floors[closingDoorFloor].floorLed, LOW);

      if (destination < currentFloor) {
        movingDownwards();
      }

      if (destination > currentFloor) {
        movingUpwards();
      }
    }
  }

  else {
    arrived = true;
  }


  if (!arrived) {
    flickerOperationalLed();

    //The doors are closed therefore we make a transition between the sound for closing doors and the sound for the moving elevator
    if (!doorsClosing) {
      tone(buzzerPin, buzzerMovingTone, 1000);
    }
  }

  else {
    flickeringLedState = HIGH;
    digitalWrite(flickeringLed, flickeringLedState);
    if (!buzzed) {
      tone(buzzerPin, buzzerArrivalTone, 1000);
      buzzed = true;
    }
  }
}



void readButtonValues() {
  //We read the states of all the buttons
  for (int i = 0; i < totalFloors; i++) {
    floors[i].buttonStateReading = digitalRead(floors[i].floorButton);
  }
}

void checkButtonPress() {

  for (int i = 0; i < totalFloors; i++) {
    if (floors[i].buttonStateReading != floors[i].lastButtonState) {
      floors[i].lastDebounceTime = millis();
    }

    for (int i = 0; i < totalFloors; i++) {
      if ((millis() - floors[i].lastDebounceTime) > debounceDelay) {
        if (floors[i].buttonStateReading != floors[i].buttonState) {
          floors[i].buttonState = floors[i].buttonStateReading;

          //The button is pressed
          if (floors[i].buttonState == LOW && arrived == true) {
            destination = i;
            lastClosingDoors = millis();
          }
        }
      }

      floors[i].lastButtonState = floors[i].buttonStateReading;
    }
  }
}


void movingDownwards() {
  /*
  The LED for the current floor has been in the HIGH state for long enough ( for the floorChangeTime period ) and we change its state
  to LOW, wishing it to stay in this state for the same time period
  */
  if (floors[currentFloor].floorLedState == HIGH && (millis() - lastFloorChange > floorChangeTime)) {
    floors[currentFloor].floorLedState = LOW;
    digitalWrite(floors[currentFloor].floorLed, floors[currentFloor].floorLedState);
    lastFloorChange = millis();
  }

  /*
  The LED for the previous floor has been in the LOW state for the desired time period therefore we continue to the next floor
  and we turn the corresponding LED on
  */
  if (floors[currentFloor].floorLedState == LOW && (millis() - lastFloorChange > floorChangeTime)) {
    currentFloor--;
    floors[currentFloor].floorLedState = HIGH;
    digitalWrite(floors[currentFloor].floorLed, floors[currentFloor].floorLedState);
    lastFloorChange = millis();
  }
}


void movingUpwards() {
  //We repeat the process in the oposite direction
  if (floors[currentFloor].floorLedState == HIGH && (millis() - lastFloorChange > floorChangeTime)) {
    floors[currentFloor].floorLedState = LOW;
    digitalWrite(floors[currentFloor].floorLed, floors[currentFloor].floorLedState);
    lastFloorChange = millis();
  }

  if (floors[currentFloor].floorLedState == LOW && (millis() - lastFloorChange > floorChangeTime)) {
    currentFloor++;
    floors[currentFloor].floorLedState = HIGH;
    digitalWrite(floors[currentFloor].floorLed, floors[currentFloor].floorLedState);
    lastFloorChange = millis();
  }
}

void flickerOperationalLed() {
  //As long as the elevator moves we flicker the LED with an interval of "movingIndicatorInterval" staying ON and OFF
  if (flickeringLedState == HIGH && (millis() - lastFlicker) > movingIndicatorInterval) {
    flickeringLedState = LOW;
    digitalWrite(flickeringLed, flickeringLedState);
    lastFlicker = millis();
  }

  if (flickeringLedState == LOW && (millis() - lastFlicker) > movingIndicatorInterval) {
    flickeringLedState = HIGH;
    digitalWrite(flickeringLed, flickeringLedState);
    lastFlicker = millis();
  }
}

void closingDoor() {
  tone(buzzerPin, buzzerClosingDoorTone, 1000);
  doorsClosing = true;
  closingDoorFloor = currentFloor;

  //As long as the "doors close" the led for that floor stays on
  digitalWrite(floors[closingDoorFloor].floorLed, HIGH);
}
