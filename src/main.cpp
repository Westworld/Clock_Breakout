#include <Arduino.h>

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
   ball->setAngle(37);
   // winkel setzen, nicht 45°
   // nicht komplett löschen und neu zeichnen, nur untere pixel?

   paddle = new Paddle(tft);
   paddle->draw();

   blocks = new Blocks(tft);
   //blocks->Setup();

  byte uhrzeit[4] = {1, 2, 3, 0};
  blocks->Setup(uhrzeit);

   blocks->draw();
   //tft->test();
   
 
}


void loop() {

  
  int16_t curBlock;


  curBlock = ball->move_draw();  
  blocks->checkBall(ball);
   
  paddle->update(ball->GetX());

    if (curBlock>=0)
  {
    blocks->drawBlock(curBlock);  // redraw empty block below moving ball 
    delay(10);
  } 
  else
    delay(10);

    

}


