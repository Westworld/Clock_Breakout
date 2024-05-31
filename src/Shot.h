#ifndef SHOT_HPP_
#define SHOT_HPP_

#include "screen.h"

#define shotwidth 4
#define shotheight 8
#define shotspeed 1

struct Shotsize
{
    float x, y, x2, y2;
    float movey;
};

class Shot
{
private:
    float move_y;
    float pos_x, pos_y;
    int16_t max_x, max_y;
    int16_t color = ILI9486_YELLOW;
    bool active=false;

    Screen * tft;

 

public:
    Shot(Screen * mastertft, int16_t movex); //, float speed, float angle);
    void draw(void);
    void undraw(void);
    int16_t move_draw(void);
    Shotsize GetSize(void);
    void activate(int16_t posx, int16_t posy, bool down);
    void deactivate(void);
    int16_t getX(void);
    int16_t getY(bool up);

};

#endif // BALL_HPP_