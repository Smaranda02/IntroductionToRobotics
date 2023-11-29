const byte rs = 9;
const byte en = 7;
const byte d4 = 6;
const byte d5 = 5;
const byte d6 = 4;
const byte d7 = 13;

const byte backLightPin = 3;
int backLightValue = 50;
int minBrightness = 0;
int maxBrightness = 100;
int minMappedValue = 0;
int maxMappedValue = 255;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


String greetingMessage = "   WELCOME TO          BOMBERMAN";
char highscoreMessage[] = "You've just set a new highscore!";
char podiumMessage[] = "You are on the podium";
