.#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

#define spaceship 0
#define spacewall 1
#define btn 9

byte ship[8] = {
  B00000,
  B00000,
  B00100,
  B11110,
  B00111,
  B11110,
  B00100,
  B00000
};

byte wall[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

int gameMap[2][16] = {
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int sceneManager = 0;
float score = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(btn, INPUT);
  
  
  // Print a message to the LCD.
  Serial.begin(9600);
  lcd.createChar(spaceship, ship);
  lcd.createChar(spacewall, wall);
}

void sceneLoad(int sceneNumber){
  switch (sceneNumber) {
    case 0:
      startScene();
      break;
    case 1:
      mainMenu();
      break;
    case 2:
        mainGame();
        break;
    case 3:
        endGame();
        break;
    default:
      // statements
      break;
  }  
}

void startScene(){
    lcd.clear();
  lcd.setCursor(0, 0);
    lcd.print("Dodge the wall");
    lcd.setCursor(6, 1);
    lcd.write(byte(spaceship));
    delay(2000);
    sceneManager = 1;
}

void mainMenu(){
    lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Btn to play");
    lcd.setCursor(0, 1);
    lcd.print("Btn to jump");
    if(digitalRead(btn)){
        sceneManager = 2;
    }
    delay(200);
  
}

int yShip = 0;

unsigned long newrandom(unsigned long howsmall, unsigned long howbig)
{
  return howsmall + random() % (howbig - howsmall);
}

void checkMoveShip(){
  if(digitalRead(btn)){
      gameMap[yShip][0] = 0;
      yShip += 1;
      yShip %= 2;
      gameMap[yShip][0] = 1;
      delay(50);
  }
}

void generateWall(){
  if(newrandom(0, 5) == 0){
    int nr = newrandom(0,2);
    int nro = (nr + 1)%2;
    if(gameMap[nro][15] != 2 && gameMap[nro][14] != 2 && gameMap[nro][13] != 2){
        gameMap[nr][15] = 2;
    }
  }
}

void printSpaceShip(int i, int j){
  lcd.setCursor(j, i);
  lcd.write(byte(spaceship));
}

void printWall(int i, int j){
  lcd.setCursor(j, i);
  lcd.write(byte(spacewall));
}

void printMap(){
  lcd.clear();
  for(int i = 0; i < 2;i++){
    for(int j = 0; j < 16;j++){
      switch (gameMap[i][j]){
        case 1:
          printSpaceShip(i,j);
          break;
        case 2:
          printWall(i,j);
          break;
        default:
          break;
      }
    }
  }
}

void moveWallToRight(){
  for(int i = 0; i < 2;i++){
    for(int j = 0; j < 16;j++){
      if(gameMap[i][j] == 2){
        gameMap[i][j] = 0;
          if(gameMap[i][j-1] == 1){
            sceneManager = 3;
              lcd.clear();
          lcd.print("Game over");
              delay(2000);
          } else {
            gameMap[i][j-1] = 2;
        }
      }
    }
  }
}


void mainGame(){
  score += 1;
  checkMoveShip();
  generateWall();
  printMap();
  moveWallToRight();
  delay(500);
  
}

void resetMap(){
  for(int i = 0; i < 2;i++){
    for(int j = 0;j < 16;j++){
      gameMap[i][j] = 0;
    }
  }
  gameMap[yShip][0] = 1;
}

void endGame(){
  lcd.clear();
  lcd.print("Btn to restart");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
  if(digitalRead(btn)){
      resetMap();
      sceneManager = 2;
      score = 0;
    }
  delay(200);
}

void loop() {
  sceneLoad(sceneManager);
  
}
 
