// Made by Christian Tsalidis --> @ctsalidis --> https://c-tsalidis.github.io
// MAX7219 - using Led Control library to display 8x8 bitmap
#include <LedControl.h>
int DIN = 11;
int CS = 7;
int CLK = 13;
LedControl lc=LedControl(DIN, CLK, CS,0);

int foodX, foodY;
int row = 0, column = 0, x = 0, y = 0, previousX = 0, previousY = 0;
int xDirection = 1, yDirection = 0;
int score = 0;

int gridSize = 8; // the grid is an 8x8 matrix of LEDs

void setup() {
  // joystick pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  // led matrix setup
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

  Serial.begin(9600);

}
void loop() {
  lc.clearDisplay(0);

  // get input from joystick
  x = map(analogRead(A0), 0, 1000, -1, 1); // read X axis value [0..1023]
  y = map(analogRead(A1), 1100, 0, -1, 1); // read Y axis value [0..1023]

  // move head
  if(x > 0 && x != previousX) { xDirection = 1; yDirection = 0; }
  else if(x < 0 && x != previousX) { xDirection = -1; yDirection = 0; }
  else if(y > 0  && y != previousY) { xDirection = 0; yDirection = 1; }
  else if(y < 0 && y != previousY) { xDirection = 0; yDirection = -1; }
  // else if(x== 0 && y == 0) { xDirection = 0; yDirection = 0; }
  row = calculateConstrainedIndex(row + xDirection); 
  column = calculateConstrainedIndex(column + yDirection);
  x = previousX;
  y = previousY;

  // check if the head is in same position as food
  if(foodX == row && foodY == column) {
    score++;
    Serial.print("Score: "); Serial.println(score);
    setFoodPos(); // reset the position of food
  }

  // draw food, head, body parts
  drawLed(foodX, foodY);
  drawLed(row, column);
  
  // Serial.print(row); Serial.print(" | "); Serial.println(column);
  delay(200);
}

// turn the led at [row, column] on
void drawLed(int row, int column) {
  lc.setLed(0,row,column,true); // lc.setLed(0,Row Number,Column number,true);
}

// in case the index is outside of the matrix array, recalculate it so it appears on the other side
int calculateConstrainedIndex(int index) {
  if (index > gridSize - 1) index = gridSize - index;
  else if (index < 0) index = gridSize + index;
  return index;
}

void setFoodPos() {
  bool foundSpot = false;
  int counter = 0;
  while(!foundSpot) {
    int randX = random(0, gridSize);
    int randY = random(0, gridSize);
    if(randX != foodX && randY != foodY) {
      foodX = randX;
      foodY = randY;
    }
    counter++;
    if(counter >= gridSize*gridSize) break; // the user won the game cause there are no free spots
  }
}
