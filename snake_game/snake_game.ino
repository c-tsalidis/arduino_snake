// Made by Christian Tsalidis --> @ctsalidis --> https://c-tsalidis.github.io
// MAX7219 - using Led Control library to display 8x8 bitmap
#include <LedControl.h>
int DIN = 11;
int CS = 7;
int CLK = 13;
LedControl lc=LedControl(DIN, CLK, CS,0);

int row = 0, column = 0, x = 0, y = 0, previousX = 0, previousY = 0;

void setup() {
  // joystick pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  // led matrix setup
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

  // Serial.begin(9600);

}
void loop() {
  lc.clearDisplay(0);
  x = map(analogRead(A0), 0, 1000, -1, 1); // read X axis value [0..1023]
  y = map(analogRead(A1), 1100, 0, -1, 1); // read Y axis value [0..1023]
  
  if(x > 0 && x != previousX) row = calculateConstrainedIndex(row + 1);
  else if(x < 0 && x != previousX) row = calculateConstrainedIndex(row - 1);
  else if(y > 0  && y != previousY) column = calculateConstrainedIndex(column + 1);
  else if(y < 0 && y != previousY) column = calculateConstrainedIndex(column - 1);
  x = previousX;
  y = previousY;
  
  drawLed(row, column);
  // Serial.print(row); Serial.print(" | "); Serial.println(column);
  delay(100);
}

// turn the led at [row, column] on
void drawLed(int row, int column) {
  lc.setLed(0,row,column,true); // lc.setLed(0,Row Number,Column number,true);
}

// in case the index is outside of the matrix array, recalculate it so it appears on the other side
int calculateConstrainedIndex(int index) {
  int gridConstrain = 7;
  if (index > gridConstrain ) index = gridConstrain + 1 - index;
  else if (index < 0) index = gridConstrain + 1 + index;
  return index;
}
