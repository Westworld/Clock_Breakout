#include <Arduino.h>
#include "SPI.h"
//#include "Waveshare_ILI9486_GFX.h"
#include "main.h"
#include "screen.h"
#include "ball.h"
#include "paddle.h"
#include "blocks.h"

#include <Console.h>


// don't forget to set in Console.h debug = none

Screen * tft;

Ball * ball;
Paddle * paddle;
Blocks * blocks;

void setup() {
   Console::begin();
  Console::line();
  // put your setup code here, to run once:
   tft = new Screen();
   ball = new Ball(tft);
   ball->setAngle(40);
   // winkel setzen, nicht 45°
   // nicht komplett löschen und neu zeichnen, nur untere pixel?

   paddle = new Paddle(tft);
   paddle->draw();

   blocks = new Blocks(tft);
   blocks->Setup();
   blocks->draw();
   //tft->test();
}


void loop() {
  ball->move_draw();
  paddle->update(ball->getPos());
  delay(2);
}