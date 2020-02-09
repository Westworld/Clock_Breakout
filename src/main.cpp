#include <Arduino.h>
#include "SPI.h"
//#include "Waveshare_ILI9486_GFX.h"
#include "main.h"
#include "screen.h"
#include "ball.h"

Screen * tft;

Ball * ball;

void setup() {
  // put your setup code here, to run once:
   tft = new Screen();
   ball = new Ball(tft);
   ball->setAngle(20);
   // winkel setzen, nicht 45°
   // nicht komplett löschen und neu zeichnen, nur untere pixel?

   tft->test();
}


void loop() {
  ball->move_draw();
  delay(2);
}