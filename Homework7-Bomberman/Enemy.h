int enemyIntensity = 5;
int moveInterval = 1000;  //each moveInterval ms the eney will change its direction
byte enemyPoints = 10;

const byte initialNoOfEnemies = 1;
byte realNoOfEnemies = initialNoOfEnemies;

class Enemy {
public:

  byte enemyLine;
  byte enemyColumn;
  unsigned long lastMove = 0;

public:
  Enemy(){}

  Enemy(byte enemyLine, byte enemyColumn){
    this->enemyLine = enemyLine;
    this->enemyColumn = enemyColumn;

  }

  void moveEnemy();
  void checkPlayerMeeting();
  void checkEnemyDamage(byte minLine, byte maxLine, byte minColumn, byte maxColumn, byte bombLine, byte bombColumn);

};

int enemiesIndex = -1;
Enemy enemies[initialNoOfEnemies];

