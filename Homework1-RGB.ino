//Lighting up a RGB Led with 3 potentiometers 

const int blueLedPin = 11;
const int redLedPin = 9;
const int greenLedPin = 10;

const int blueLedPotentiometerPin = A0;
const int redLedPotentiometerPin = A2;
const int greenLedPotentiometerPin = A1;

int blueLedPotentiometer = 0;
int redLedPotentiometer = 0;
int greenLedPotentiometer = 0;

float voltageBlue = 0;
float voltageRed = 0;
float voltageGreen = 0;

const int maxMappedVoltage = 255;
const int minMappedVoltage = 0;

const int maxUnmappedValue = 1023;
const int minUnmappedValue = 0; 

const int delayTime = 3;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}


void loop() {
  // read the input on analog pins 0,1,2:
 readPotValues();
 mapValues();
 lightLED();
 // delay in between reads for stability
 delay(delayTime);  
}

void readPotValues(){
  blueLedPotentiometer = analogRead(blueLedPotentiometerPin); 
  redLedPotentiometer = analogRead(redLedPotentiometerPin);
  greenLedPotentiometer = analogRead(greenLedPotentiometerPin);
}

//Map the values from potentiometers [0,1023] to [0,255] (values accepted by analogWrite function)
void mapValues(){
  voltageBlue = map(blueLedPotentiometer,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);   
  voltageRed = map(redLedPotentiometer,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);
  voltageGreen = map(greenLedPotentiometer,minUnmappedValue,maxUnmappedValue,minMappedVoltage,maxMappedVoltage);
}


void lightLED(){
  analogWrite(blueLedPin, voltageBlue);
  analogWrite(redLedPin, voltageRed);
  analogWrite(greenLedPin, voltageGreen);
}

