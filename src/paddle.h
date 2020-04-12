#ifndef PADDLE_HPP_
#define PADDLE_HPP_

#include "screen.h"
#include "ball.h"

class Paddle
{
private:
    float pos_x;
#ifdef TARGET_8266
    int16_t pos_y=10;
#endif   
#ifdef TARGET_esp32
    int16_t pos_y=0;
#endif 
    int16_t max_x, max_y;
    const int16_t paddlewidth=32;
    const int16_t paddleheight=4;
    uint16_t color = ILI9486_YELLOW;

    Screen * tft;


public:
    Paddle(Screen * mastertft);
    void update(float ballpos);
    void draw(void);
    int16_t getX(void);
    void setX(int16_t x);
};

#endif // PADDLE_HPP_