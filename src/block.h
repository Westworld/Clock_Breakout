#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "screen.h"

#define blockwidth 28
#define blockheight 22



class Block
{
private:
    int16_t pos_x, pos_y;
    uint16_t color = ILI9486_YELLOW;
    bool used=false;

public:
   bool active=false;

    Block(void);
    void activate(int16_t posx, int16_t posy) ;
    //void update(float ballpos);;
    void draw(Screen * mastertft);
    int16_t check(int16_t posx, int16_t pos_y, int16_t move_x, int16_t move_y, Screen * mastertft);
};

#endif // BLOCK_HPP_