#ifndef PADDLE_HPP_
#define PADDLE_HPP_

#include "screen.h"
#include "ball.h"

class Paddle
{
private:
    float pos_x;
    int16_t pos_y=0;
    int16_t max_x, max_y;
    const int16_t paddlewidth=32;
    const int16_t paddleheight=4;
    uint16_t color = ILI9486_YELLOW;

    Screen * tft;


public:
    Paddle(Screen * mastertft);
    void update(float ballpos);
    void draw(void);
};

#endif // PADDLE_HPP_