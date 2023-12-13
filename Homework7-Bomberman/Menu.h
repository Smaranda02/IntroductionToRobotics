enum MenuOrder {MainMenu=0,  StartMenu, AboutGameMenu, HighscoreMenu, GameSettingsMenu, InstructionsMenu, GameRestartMenu, BrightnessControlMenu,
                EnterNameMenu, SaveNameMenu, SoundMenu};

const char PROGMEM startGame[] = "Start Game";
const char PROGMEM about[] = "About";
const char PROGMEM highScores[] = "Highscores";
const char PROGMEM settings[] = "Settings";
const char PROGMEM howToPlay[] = "How to play";
byte mainMenuSize = 5;
const char* const PROGMEM MENU[] = { startGame, about, highScores, settings, howToPlay };


const char PROGMEM startGameYES[] = "YES";
const char PROGMEM startGameNO[] = "NO";
byte startGameMenuSize = 2;
const char* const PROGMEM StartGame[] = { startGameYES, startGameNO };

const char PROGMEM aboutGameName[] = "Game name: Unabomber";
const char PROGMEM aboutGameCreator[] = "Game creator: Smaranda";
const char PROGMEM aboutGithub[] = "Github: Smaranda02";
const char PROGMEM aboutBack[] = "Back";
byte aboutMenuSize = 4;
const char* const PROGMEM About[] = { aboutGameName, aboutGameCreator, aboutGithub, aboutBack };


const char PROGMEM string1[] = "Place bombs to";
const char PROGMEM string2[] = "destroy walls.";
const char PROGMEM string3[] = "Run from the";
const char PROGMEM string4[] = "enemies.Explore";
const char PROGMEM string5[] = "all the rooms.";
const char PROGMEM string6[] = "Be fast and aim";
const char PROGMEM string7[] = "for #1.Have fun";
const char PROGMEM string8[] = "Back";
byte howToMenuSize = 8;
const char* const PROGMEM HowTo[] = { string1, string2, string3, string4, string5, string6, string7, string8 };

const char PROGMEM restart[] = "Restart";
const char PROGMEM backToMenu[] = "Menu";
byte restartGameMenuSize = 2;
const char* const PROGMEM RestartGame[] = { restart, backToMenu };

const char PROGMEM save[] = "Save";
byte saveNameMenuSize = 2;
const char* const PROGMEM EnterName[] = { save, string8 };

const char PROGMEM settingsLCD[] = "LCD Brightness";
const char PROGMEM settingsMatrix[] = "Matrx Brightness";
const char PROGMEM enterName[] = "Enter your name";
const char PROGMEM sound[] = "Sound";
const char PROGMEM back[] = "Back";
byte settingsMenuSize = 5;
const char* const PROGMEM Settings[] = { settingsLCD, settingsMatrix, enterName, sound, back };

byte highscoresMenuSize = 4;

const char PROGMEM joystickControl[] = "Move UP / DOWN";
byte joystickControlMenuSize = 2;
const char* const PROGMEM JoystickControl[] = { joystickControl, back };
byte controlVariable = 0;  //0 = LCD, 1 = Matrix

bool inMenu = true;
bool greeted = false;
unsigned long startGreet = 0;

const char* StartGameMessage = "  Wanna start?  ";

const char* arrow = ">";

int currentMenu = 0;
int currentOption = 0;

unsigned int scrollTime = 300;
unsigned long lastScroll = 0;
volatile bool inScroll = false;
byte scrollIndex = 0;
volatile bool scrollOnFirst = false;
volatile bool scrollOnSecond = false;

byte menuSize;
char** menuPointer = MENU;

byte lcdBrightnessMemoryLocation = 0;
byte matrixBrightnessMemoryLocation = 1;
byte firstPlayerMemoryLocation = 10;
byte secondPlayerMemoryLocation = 20;
byte thirdPlayerMemoryLocation = 30;

/*

Game flow :
Level 1 :
  - Room full of walls, you need to distroy all the walls in 120 seconds in order to reach level 2
  - After lvl 1 


EEPROM Map
0 -> LCD Brightness
1 -> Matrix Brightness

10 -> #1
20 -> #2
30 -> #3
40 -> Sound Value 


Rooms map 

----------------


*/