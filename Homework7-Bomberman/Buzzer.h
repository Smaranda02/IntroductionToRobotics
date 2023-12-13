//buzzer
const byte buzzerPin = 8;
unsigned int buzzerTone = 1000;
unsigned int buzzerDuration = 500;

byte soundMemoryLocation = 40;
volatile bool soundValue = HIGH;

struct Sound {
  unsigned int duration;
  unsigned int pitch;
};
 
Sound Melody[] = {
  {1000, 300},
  {1000, 400},
  {1000, 500}
};
 
//playMelody(Melody, sizeof(Melody) / sizeof(Melody[0]));
 