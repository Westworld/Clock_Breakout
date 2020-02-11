#include "Block.h"

#include <Console.h>


Block::Block(void)
{
    used=false;
    active=false;
}

void Block::activate(int16_t posx, int16_t posy) 
{
    used = true;
    active = true;

    pos_x = posx;
    pos_y = posy;

    //Console::info("Block pos [%d] [%d]: ", pos_x, pos_y);
}

void Block::draw(Screen * tft)
{
    if (used) {
        tft->fillRect(pos_x, pos_y, blockwidth, blockheight, color);
    } 
}