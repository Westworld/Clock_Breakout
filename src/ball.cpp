#include "Ball.h"

Ball::Ball(Screen * mastertft)
{
    tft = mastertft;
    pos_x = tft->getwidth() / 2;

 #ifdef TARGET_esp32    
    pos_y = 10;
#endif
#ifdef TARGET_8266
    pos_y = 18;
#endif    
    move_x = 1;
    move_y = 1;
    max_x = tft->getwidth();
    max_y = tft->getheight();
}

int16_t Ball::GetX(void) {
    return pos_x;
}

void Ball::SetY(int16_t y) {
    pos_y = y;
}

Ballsize Ball::GetSize(void) {
    Ballsize thesize;
/*
    if (move_x > 0) 
        { thesize.x = pos_x; thesize.x2 = pos_x+radius; }
    else 
        { thesize.x2 = pos_x; thesize.x = pos_x-radius; }

    if (move_y > 0) 
        { thesize.y = pos_y; thesize.y2 = pos_y+radius; }
    else 
        { thesize.y2 = pos_y; thesize.y = pos_y-radius; }
*/

    thesize.x = pos_x; thesize.x2 = pos_x+radius;
    thesize.y = pos_y; thesize.y2 = pos_y+radius;
    
    thesize.movex = move_x;
    thesize.movey = move_y;

    return thesize;
}

void Ball::SetMove(float x, float y) {

//Console::info("set move x [%d] y [%F] ", x, y);

    move_x = x;
    move_y = y;
}

void Ball::InvertMove(bool x, bool y) {
 if (x) move_x = -move_x;
 if (y) move_y = -move_y;
}

void Ball::SetBlock(int16_t index) {
    blockid = index;
}

void Ball::draw(void) {
    int16_t x, y;
    x = (int16_t) pos_x;
    y = (int16_t) pos_y;    
    tft->fillRect(x, y, radius, color);
    //tft->fillCircle(pos_x, pos_y, radius, color);
}

void Ball::undraw(void){
    int16_t x, y;
    x = (int16_t) pos_x;
    y = (int16_t) pos_y; 
    tft->fillRect(x, y, radius, ILI9486_BLACK);
}

void Ball::move(int16_t x, int16_t y) {
    pos_x = pos_x + x;
    pos_y = pos_y + y;
}

void Ball::move_draw(int16_t x, int16_t y) {
    this->undraw();
    pos_x = pos_x + x;
    pos_y = pos_y + y;
    this->draw();
}



int16_t Ball::move_draw(void) {
    this->undraw();
    if (move_x == 0) move_x = 0.5;
    pos_x = pos_x + move_x;
    pos_y = pos_y + move_y;

    if ((pos_x <= 0) || (pos_x >= max_x)) move_x = -move_x;
  

#ifdef TARGET_esp32    
    if ((pos_y <= 4) || (pos_y >= max_y)) move_y = -move_y;
#endif
#ifdef TARGET_8266
    if ((pos_y <= 15) || (pos_y >= max_y)) move_y = -move_y;
#endif 

    this->draw();  
    return blockid; 
}

void Ball::setAngle(float angle)
{
    move_x = speed * cos(angle * M_PI / 180);
    move_y = -speed * sin(angle * M_PI / 180);
}
float Ball::getAngle()
{
    // angle in range [-180; 180]
    float angle = atan2(-move_y, move_x) * 180 / M_PI;
    // angle in range [0; 360]
    if (angle < 0.f)
        angle += 360.f;
    return angle;
}
