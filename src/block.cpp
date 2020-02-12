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
        if (active)
            tft->fillRect(pos_x, pos_y, blockwidth, blockheight, color);
        else
        {
            tft->fillRect(pos_x, pos_y, blockwidth, blockheight, ILI9486_BLACK);
        }
            
    } 
}

bool Block::check(int16_t posx, int16_t posy, int16_t movex, int16_t movey, Screen * tft)
{
    bool result=false;

    if (used && active) {
            if ( (posx <= (pos_x+blockwidth)) && (posx >= pos_x) 
                && (posy <= (pos_y+blockheight)) && (posy >= pos_y) )
                {
                    active=false;
                    draw(tft);
                }
  
        
    }

    return result;
}