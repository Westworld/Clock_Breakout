#include "Ball.h"


Ball::Ball(Screen * mastertft)
{
    tft = mastertft;
    pos_x = tft->getwidth() / 2;
    pos_y = 6;
    move_x = 1;
    move_y = 1;
    max_x = tft->getwidth();
    max_y = tft->getheight();
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
    //tft->fillCircle(pos_x, pos_y, radius, ILI9486_BLACK);
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

void Ball::move_draw(void) {
    this->undraw();
    pos_x = pos_x + move_x;
    pos_y = pos_y + move_y;
    if ((pos_x <= 0) || (pos_x >= max_x)) move_x = -move_x;
    if ((pos_y <= 0) || (pos_y >= max_y)) move_y = -move_y;
    this->draw();        

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
