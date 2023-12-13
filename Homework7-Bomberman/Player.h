//player variables
unsigned int playerBlinkTime = 400;
unsigned long lastPlayerBlink = 0;
volatile bool playerState = LOW;
byte noOfLives = 1;
byte lives = 1;
volatile bool isAlive = true;
byte score = 0;
byte oldScore = score;
byte startTime = 60;
byte time = startTime;
volatile bool stopTimeCount = false;
byte level = 1;
unsigned long lastTimeCount = 0;
unsigned int timeDelay = 1000;
volatile bool highscoreDisplayed = false;
volatile bool podiumDisplayed = false;
unsigned int messageDisplayTime = 2000;
unsigned long lastMessageDisplay = 0;
volatile bool anotherMessage = false;

volatile bool inGame = false;

const byte maxNameCharacters = 5;
byte maxPodiumPlayers = 3;

struct AwardedPlayer {
  const char playerName[maxNameCharacters];
  byte highscore;
};

//player position
byte playerLine = 0;
byte playerColumn = 0;
char name[maxNameCharacters] = "     ";
int nameIndex = -1;
