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
    int16_t paddleheight=4;
    uint16_t color = ILI9486_YELLOW;
    bool justLine=true;

    Screen * tft;


public:
    Paddle(Screen * mastertft);
    void setType(bool gametyp);
    void update(float ballpos);
    void move(int16_t movex);
    void draw(void);
    void undraw(void);
    int16_t getX(void);
    void setX(int16_t x);
};

#endif // PADDLE_HPP_