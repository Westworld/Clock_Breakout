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
    bool active=false;
    bool used=false;




public:
    Block(void);
    void activate(int16_t posx, int16_t posy) ;
    //void update(float ballpos);;
    void draw(Screen * mastertft);
};

#endif // BLOCK_HPP_