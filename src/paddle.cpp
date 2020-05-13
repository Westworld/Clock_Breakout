#include "paddle.h"


Paddle::Paddle(Screen * mastertft)
{
    tft = mastertft;
    pos_x = tft->getwidth() / 2;
    max_x = tft->getwidth();
    max_y = tft->getheight();
}

void Paddle::setType(bool gametyp)
{
    justLine = gametyp;
    if (gametyp) {
        paddleheight = 4;
    }
    else
    {
        paddleheight = 8;    
    }
    
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

    if (justLine)
        tft->fillRect(x, pos_y, paddlewidth, paddleheight, ILI9486_BLACK);
    else {
        tft->fillRect(x, pos_y, paddlewidth, paddleheight, ILI9486_BLACK);
    }    
}

void Paddle::draw(void)
{
    int16_t x = pos_x - (paddlewidth/2);
    if (x<0)
        x=0;
    else if (x+paddlewidth > max_x)
            x = max_x-paddlewidth;

    if (justLine)
        tft->fillRect(x, pos_y, paddlewidth, paddleheight, color);
    else {
        tft->fillRect(x, pos_y, paddlewidth, paddleheight, color);
    }    
}

void Paddle::move(int16_t movex)  // only for block
{

    if (movex == 0) return;

    int16_t x = pos_x - (paddlewidth/2);

    if (x<0)
        x=0;
    else if (x+paddlewidth > max_x)
        x = max_x-paddlewidth;

    if (movex < 0) {
            // move to the right, remove on the left. left is higher number!
            tft->fillRect(x+paddlewidth+1, pos_y, abs(movex)+1, paddleheight, ILI9486_BLACK);
            pos_x += movex;
            x += movex;
            tft->fillRect(x, pos_y, abs(movex), paddleheight, ILI9486_YELLOW);
        } else {
            tft->fillRect(x - movex, pos_y, movex+1, paddleheight, ILI9486_BLACK);  // +paddlewidth
            pos_x += movex;
            x += movex;
            tft->fillRect(x+paddlewidth, pos_y, movex, paddleheight, color);
        }
}

void Paddle::update(float ballpos)  // only for justLine
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