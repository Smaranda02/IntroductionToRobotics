//Lighting up a RGB Led with 3 potentiometers 

const int blueLedPin = 11;
const int redLedPin = 9;
const int greenLedPin = 10;

const int potPinForBlueLed = A0;
const int potPinForRedLed = A2;
const int potPinForGreenLed = A1;

int potValueForBlueLed = 0;
int potValueForRedLed = 0;
int potValueForGreenLed = 0;

float voltageBlue = 0;
float voltageRed = 0;
float voltageGreen = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}


void loop() {
  // read the input on analog pins 0,1,2:
  potValueForBlueLed = analogRead(potPinForBlueLed); 
  potValueForRedLed = analogRead(potPinForRedLed);
  potValueForGreenLed = analogRead(potPinForGreenLed);

  //We map the values output by the potentiometers which are in the interval [0,1023] to the interval [0,255] (values accepted 
  //by analogWrite function)
  
  voltageBlue = map(potValueForBlueLed,0,1023,0,255);   
  voltageRed = map(potValueForRedLed,0,1023,0,255);
  voltageGreen = map(potValueForGreenLed,0,1023,0,255);

  Serial.println(voltageRed);
  analogWrite(blueLedPin, voltageBlue);
  analogWrite(redLedPin, voltageRed);
  analogWrite(greenLedPin, voltageGreen);

  delay(3);  // delay in between reads for stability
}
