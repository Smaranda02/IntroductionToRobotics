const byte rs = 9;
const byte en = 7;
const byte d4 = 6;
const byte d5 = 5;
const byte d6 = 4;
const byte d7 = 13;

const byte backLightPin = 3;
byte backLightValue = 55;
byte minBrightness = 0;
byte maxBrightness = 100;
byte minMappedValue = 0;
byte maxMappedValue = 255;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


bool shown = false;
unsigned int letterBlinkTime = 400;
unsigned long lastLetterBlink = 0;
volatile bool letterState = LOW;

String greetingMessage = "   WELCOME TO          BOMBERMAN";
unsigned int greetingDisplayTime = 1500;
const char PROGMEM highscoreMessage[] = "You've just set a new highscore!";

char alphabet1[] = "ABCDEFGHIJKLMNOP";
char alphabet2[] = "QRSTUVWXYZ < Y/N";
char currentLetterValue;  //A
byte currentLetter = 0;   // letter index



const char PROGMEM gameOverMessage[] = "Game over...";
const char PROGMEM endGameMessage[] = "Congrats! You completed the game!";