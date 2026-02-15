#include <LedControl.h>

// Pines y Configuración
const int DIN =  11, CS = 10, CLK = 13;
const int btnL = 6, btnR = 5, btnS = 3;
const int buzzer = 4;
const int WIDTH = 8;
// Variables de posición (0 a 7 para la librería)
int playerX = 4; 
int playerY = 7; 
int ballX = WIDTH/2;
int ballY = WIDTH/2;
int dirX = 0;
int dirY = 1;
unsigned long ballClock = 0;
LedControl lc = LedControl(DIN, CLK, CS, 1);

void setup() {
  pinMode(btnL, INPUT_PULLUP);
  pinMode(btnR, INPUT_PULLUP);
  pinMode(btnS, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  
  renderPlayer(true); // Dibujar posición inicial
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
  checkInput(); // Movimiento de plataforma (siempre activo)

  // Control de tiempo para la pelota
  if (millis() - ballClock > 250) { 
    updateGame(); 
    ballClock = millis();
  }
}

void checkInput() {
  if (digitalRead(btnL) == LOW) {
    if (playerX > 1) { // Evitar que se salga por la izquierda (cuerpo de 2 leds)
      renderPlayer(false); // Borrar donde estaba
      playerX--;           // Mover
      renderPlayer(true);  // Dibujar en nuevo sitio
      delay(150);          // Antirebote
    }
  }

  if (digitalRead(btnR) == LOW) {
    if (playerX < 7) {     // Evitar que se salga por la derecha
      renderPlayer(false); 
      playerX++;
      renderPlayer(true);
      delay(150);
    }
  }
}

// Una sola función para pintar o borrar
// Usamos playerX y playerY globales para no confundir variables
void renderPlayer(bool state) {
  // El jugador ocupa dos puntos: (X, Y) y (X-1, Y)
  lc.setLed(0, playerY, playerX, state);
  lc.setLed(0, playerY, playerX - 1, state);
}

void renderBall(bool state) {
  lc.setLed(0, ballY, ballX, state);
}

void updateGame() {
  // 1. BORRAR SOLO LA POSICIÓN VIEJA
  renderBall(false);

  // 2. CALCULAR NUEVA POSICIÓN PROYECTADA
  int proxX = ballX + dirX;
  int proxY = ballY + dirY;

  // 3. REBOTE EN PAREDES
  if (proxX > 7 || proxX < 0) {
    dirX = -dirX;
    proxX = ballX + dirX; 
  }
  if (proxY < 0) {
    dirY = -dirY;
    proxY = ballY + dirY;
  }

  // 4. REBOTE EN PLATAFORMA (CORREGIDO)
  if (proxY == playerY && (proxX == playerX || proxX == playerX - 1)) {
    dirY = -1; // Forzamos dirección hacia arriba
    
    if (dirX == 0) {
        dirX = (playerX > 4) ? -1 : 1;
    }
    
    // IMPORTANTE: La pelota se queda en la fila 6, NO en la 7
    proxY = playerY - 1; 
    proxX = ballX + dirX; // Recalculamos X con el nuevo dirX
  } 
  
  // 5. GAME OVER
  else if (proxY > 7) {
    gameOver();
    resetGame();
    return;
  }

  // 6. ACTUALIZAR Y DIBUJAR
  ballX = proxX;
  ballY = proxY;
  
  // Limitar ballX para que no se salga de la matriz por el rebote
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
  tone(buzzer, 146, 600); // Re (D3) - Nota larga final
  delay(700);
  noTone(buzzer);         // Asegurar que silencia
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
  // Notas rápidas ascendentes: Do4, Mi4, Sol4, Do5
  tone(buzzer, 261, 100); // Do (C4)
  delay(150);
  tone(buzzer, 329, 100); // Mi (E4)
  delay(150);
  tone(buzzer, 392, 100); // Sol (G4)
  delay(150);
  tone(buzzer, 523, 300); // Do (C5) - Nota aguda final
  delay(400);
  noTone(buzzer);
}