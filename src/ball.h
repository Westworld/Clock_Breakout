#ifndef BALL_HPP_
#define BALL_HPP_

#include "screen.h"


class Ball
{
private:
    float move_x, move_y;
    float pos_x, pos_y;
    int16_t max_x, max_y;
    int16_t speed = 1;
    const int16_t radius=8;
    uint16_t color = ILI9486_YELLOW;

    Screen * tft;


public:
    Ball(Screen * mastertft); //, float speed, float angle);
    void draw(void);
    void undraw(void);
    void move(int16_t x, int16_t y);
    void move_draw(int16_t x, int16_t y);
    void move_draw(void);
    void setAngle(float angle);
    float getAngle();
    float getPos();
};

#endif // BALL_HPP_