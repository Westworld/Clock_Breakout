#include <Arduino.h>
#include "SPI.h"
//#include "Waveshare_ILI9486_GFX.h"
#include "main.h"
#include "screen.h"
#include "ball.h"
#include "paddle.h"

Screen * tft;

Ball * ball;
Paddle * paddle;

void setup() {
  // put your setup code here, to run once:
   tft = new Screen();
   ball = new Ball(tft);
   ball->setAngle(20);
   // winkel setzen, nicht 45°
   // nicht komplett löschen und neu zeichnen, nur untere pixel?

   paddle = new Paddle(tft);
   paddle->draw();
   //tft->test();
}


void loop() {
  ball->move_draw();
  paddle->update(ball->getPos());
  delay(2);
}