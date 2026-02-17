/*
Hey everyone, and welcome to my repository. Here, i made a program that you can load into the arduino nano
that's inside of the splash nano, a 'console' i just created by myself. It consists on 1 arduino nano, 1x 1088AS, 3x buttons, 1x switch or interruptor and a buzzer.
The first game to ever exist on it is this one, which is the only one i created so far. I will try to make a tetris, with/without music, and when i have more time,
i'll just tune it, even tho i took 4 days to create the first one (1st day was research, 2nd was programming this, 3rd testing and debugging and finally, on day 4th, i finished it by welding all the things
together.). I'll just take some rest and develop more recreations of games. This one is like arkanoid, but it's not the same, since in arkanoid, there are blocks that you break, but so far, i just made
the player and a bouncing ball.
*/


#include <LedControl.h>

// Pins and configuration
const int DIN =  11, CS = 10, CLK = 13;
const int btnL = 6, btnR = 5, btnS = 3;
const int buzzer = 4;
const int WIDTH = 8;
const int speedLimit = 100; // Change it however you want
const int acceleration = 10;
// Position variables + library fundamental variables
int playerX = 4; 
int playerY = 7; 
int ballX = WIDTH/2;
int ballY = WIDTH/2;
int dirX = 0;
int dirY = 1;
int speed = 250;
unsigned long ballClock = 0;
LedControl lc = LedControl(DIN, CLK, CS, 1);

void setup() {
  pinMode(btnL, INPUT_PULLUP);
  pinMode(btnR, INPUT_PULLUP);
  pinMode(btnS, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  
  renderPlayer(true); // Draw initial player
  renderBall(true);
  bool ready = false;
  while (!ready) {
    if (digitalRead(btnS) == LOW) {
      break;
    }
  }
  startMusic();
}

void loop() {
  checkInput(); // Ball movement (Always active)

  // Time control for the ball
  if (millis() - ballClock > speed) { 
    updateGame(); 
    ballClock = millis();
  }
}

void checkInput() {
  if (digitalRead(btnL) == LOW) {
    if (playerX > 1) { // Avoid the player from disappearing in the left
      renderPlayer(false); 
      playerX--;        
      renderPlayer(true); 
      delay(150);
    }
  }

  if (digitalRead(btnR) == LOW) {
    if (playerX < 7) {     // Avoid the player from disappearing
      renderPlayer(false); 
      playerX++;
      renderPlayer(true);
      delay(150);
    }
  }
}

// Only 1 function to erase or draw the player
void renderPlayer(bool state) {
  // El jugador ocupa dos puntos: (X, Y) y (X-1, Y)
  lc.setLed(0, playerY, playerX, state);
  lc.setLed(0, playerY, playerX - 1, state);
}

void renderBall(bool state) {
  lc.setLed(0, ballY, ballX, state);
}

void updateGame() {
  // Erase the old ball
  renderBall(false);

  
  int proxX = ballX + dirX;
  int proxY = ballY + dirY;

  
  if (proxX > 7 || proxX < 0) {
    dirX = -dirX;
    proxX = ballX + dirX; 
  }
  if (proxY < 0) {
    dirY = -dirY;
    proxY = ballY + dirY;
  }

  
  if (proxY == playerY && (proxX == playerX || proxX == playerX - 1)) {
    dirY = -1; // Force direction up
    
    if (dirX == 0) {
        dirX = (playerX > 4) ? -1 : 1;
    }
    
    // IMPORTANT: The ball stays on the row number 6, not in 7
    proxY = playerY - 1; 
    proxX = ballX + dirX; 
    if (speed > speedLimit) {
      speed -= acceleration;
    }
  } 
  
  
  else if (proxY > 7) {
    gameOver();
    resetGame();
    return;
  }

  ballX = proxX;
  ballY = proxY;
  
  if(ballX < 0) ballX = 0;
  if(ballX > 7) ballX = 7;

  renderBall(true);
  renderPlayer(true); 
}


void gameOver() {
  tone(buzzer, 196, 300); // Sol (G3)
  delay(350);
  tone(buzzer, 174, 300); // Fa (F3)
  delay(350);
  tone(buzzer, 164, 300); // Mi (E3)
  delay(350);
  tone(buzzer, 146, 600); // Re (D3) - Large note at the end
  delay(700);
  noTone(buzzer);         // Makes sure it shuts up
}

void resetGame() {
  playerX = 4; 
  playerY = 7; 
  ballX = WIDTH/2;
  ballY = WIDTH/2;
  dirX = 0;
  dirY = 1;
  bool replay = false;
  lc.setRow(0, 7, 0x00);
  while (!replay) {
    if (digitalRead(btnS) == LOW) {
      break;
    }
  }
  startMusic();
}

void startMusic() {
  // Rapidly asecnding notes: Do4, Mi4, Sol4, Do5
  tone(buzzer, 261, 100); // Do (C4)
  delay(150);
  tone(buzzer, 329, 100); // Mi (E4)
  delay(150);
  tone(buzzer, 392, 100); // Sol (G4)
  delay(150);
  tone(buzzer, 523, 300); // Do (C5) - Sharp note at the end
  delay(400);
  noTone(buzzer);
}


// Congratulations, you've made it to the end of this repository.
// Go to 95.131.202.52 just to know what the splash nano is.
// Basically, it's a light console made using an arduino nano, a 1088AS, a buzzer, 3 buttons and 1 interruptor / switch
// I am also the creator of it. Thanks for distributing mi code.
