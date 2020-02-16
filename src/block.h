#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "screen.h"
#include "ball.h"

#define blockwidth 23
#define blockheight 20

class Block
{
private:
    int16_t pos_x, pos_y;
    uint16_t color = ILI9486_YELLOW;

public:
   bool active=false;
    bool used=false;

    Block(void);
    void activate(int16_t posx, int16_t posy) ;
    //void update(float ballpos);;
    void draw(Screen * mastertft);
    void undraw(Screen * mastertft);
    int16_t check(int16_t posx, int16_t pos_y, int16_t move_x, int16_t move_y, Screen * mastertft);
    bool check(Ballsize ballsize, Ball * ball, Screen * tft);
};

#endif // BLOCK_HPP_