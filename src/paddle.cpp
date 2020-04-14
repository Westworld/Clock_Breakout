#include "paddle.h"


Paddle::Paddle(Screen * mastertft)
{
    tft = mastertft;
    pos_x = tft->getwidth() / 2;
    max_x = tft->getwidth();
    max_y = tft->getheight();
}


int16_t Paddle::getX(void) {
    return pos_x;
}

void Paddle::setX(int16_t x) {
    pos_x = x;
}

void Paddle::undraw(void)
{
    int16_t x = pos_x - (paddlewidth/2);
    if (x<0)
        x=0;
    else if (x+paddlewidth > max_x)
            x = max_x-paddlewidth;

    tft->fillRect(x, pos_y, paddlewidth, paddleheight, ILI9486_BLACK);
}

void Paddle::draw(void)
{
    int16_t x = pos_x - (paddlewidth/2);
    if (x<0)
        x=0;
    else if (x+paddlewidth > max_x)
            x = max_x-paddlewidth;

    tft->fillRect(x, pos_y, paddlewidth, paddleheight, color);
}

void Paddle::update(float ballpos)
{
    int16_t half = paddlewidth / 2;
    int16_t old = (uint16_t) pos_x;

    if (ballpos < pos_x) {
        pos_x--;
        if (pos_x - half < 0)
            pos_x = half;
    }
    else {
        pos_x++;
        if (pos_x + half > max_x)
            pos_x = max_x-half;
    }

    if (pos_x == old) {
        // nothing
    } else
    {
        if (pos_x < old) {
            // remove on the right, draw on the left
            tft->fillRect(old + half, pos_y, 2, paddleheight, ILI9486_BLACK);
            tft->fillRect(pos_x - half, pos_y, 2, paddleheight, color);
        } else {
            tft->fillRect(old - half, pos_y, 2, paddleheight, ILI9486_BLACK);
            tft->fillRect(pos_x + half, pos_y, 2, paddleheight, color);
        }
    }

}