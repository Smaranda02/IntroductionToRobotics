class Bomb {

public:
  int bombBlinkingTime = 200;
  unsigned long startExplosion=0;
  int explosionTime = 700;
  unsigned long lastBombBlink = 0;
  volatile bool bombState = LOW;
  int bombLine;
  int bombColumn;
  unsigned long bombCoutdownStart = 0;
  byte actionArea = 2;
  int bombDetonationTime = 4000;
  volatile bool buzzed = true;
  bool isDetonated = true;

public:
  Bomb() {}

  Bomb(int bombLine, int bombColumn, unsigned long bombCoutdownStart, bool buzzed, unsigned long lastBombBlink) {
    this->bombLine = bombLine;
    this->bombColumn = bombColumn;
    this->bombCoutdownStart = bombCoutdownStart;
    this->buzzed = buzzed;
    this->lastBombBlink = lastBombBlink;
  }

  void checkBombState();
  void blinkBomb();
  void explodeSound();
  void explodeBomb();
  void distroyWalls();
};
