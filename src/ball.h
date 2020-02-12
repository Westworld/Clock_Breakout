#ifndef BALL_HPP_
#define BALL_HPP_

#include "screen.h"

struct Ballsize
{
    int16_t x, y, x2, y2, movex, movey;
};

class Ball
{
private:
    float move_x, move_y;
    float pos_x, pos_y;
    int16_t max_x, max_y;
    int16_t speed = 1;
    const int16_t radius=8;
    uint16_t color = ILI9486_YELLOW;
    uint16_t blockid = 0;

    Screen * tft;


public:
    Ball(Screen * mastertft); //, float speed, float angle);
    void draw(void);
    void undraw(void);
    void move(int16_t x, int16_t y);
    void move_draw(int16_t x, int16_t y);
    void move_draw(void);
    //void move_draw(Blocks * blocks);
    void setAngle(float angle);
    float getAngle();
    int16_t GetX(void);
    Ballsize GetSize(void);
    void SetBlock(int16_t index);
    
};

#endif // BALL_HPP_