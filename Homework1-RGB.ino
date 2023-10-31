//Lighting up a RGB Led with 3 potentiometers 

const int blueLedPin = 11;
const int redLedPin = 9;
const int greenLedPin = 10;

const int variableResistorPinForBlueLed = A0;
const int variableResistorPinForRedLed = A2;
const int variableResistorPinForGreenLed = A1;

int variableResistorValueForBlueLed = 0;
int variableResistorValueForRedLed = 0;
int variableResistorValueForGreenLed = 0;

float voltageBlue = 0;
float voltageRed = 0;
float voltageGreen = 0;

int maxMappedVoltage = 255;
int minMappedVoltage = 0;

int maxUnmappedValue = 1023;
int minUnmappedValue = 0; 

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}


void loop() {
  // read the input on analog pins 0,1,2:
  variableResistorValueForBlueLed = analogRead(variableResistorPinForBlueLed); 
  variableResistorValueForRedLed = analogRead(variableResistorPinForRedLed);
  variableResistorValueForGreenLed = analogRead(variableResistorPinForGreenLed);

  //We map the values output by the potentiometers which are in the interval [0,1023] to the interval [0,255] (values accepted 
  //by analogWrite function)
  voltageBlue = map(variableResistorValueForBlueLed,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);   
  voltageRed = map(variableResistorValueForRedLed,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);
  voltageGreen = map(variableResistorValueForGreenLed,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);

  Serial.println(voltageRed);
  analogWrite(blueLedPin, voltageBlue);
  analogWrite(redLedPin, voltageRed);
  analogWrite(greenLedPin, voltageGreen);

  delay(3);  // delay in between reads for stability
}
