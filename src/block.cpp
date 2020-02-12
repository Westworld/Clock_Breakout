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

int16_t Block::check(Ballsize ballsize, Ball * ball, Screen * tft) {
   int16_t result=0;

    if (used) {
        if ( (ballsize.x <= (pos_x+blockwidth)) && (ballsize.x2 >= pos_x) 
            && (ballsize.y <= (pos_y+blockheight)) && (ballsize.y2 >= pos_y) )
            {
                if (!active) 
                {
                    return -1;
                }
                else
                {
                    active=false;
                    draw(tft);
                /*
                // calculate new reflection angle
                if (movex<0)  // moving left
                {    if (posx <= (pos_x+blockwidth))
                    { result = 1; return result;  }
                }
                else
                {    if (posx >= pos_x)
                    { result = 2; return result;  }
                }

                if (movey<0)  // moving down
                {    if (posy <= (pos_y+blockheight))
                    { result = 3; return result;  }
                }
                else
                {    if (posy >= pos_y)
                    { result = 4; return result;  }
                }   
                return 0;  // should never happen! pass thru...
                */  
                }         
            }       
    }

    return result;

}

int16_t Block::check(int16_t posx, int16_t posy, int16_t movex, int16_t movey, Screen * tft)
{
    int16_t result=0;

    if (used && active) {
        if ( (posx <= (pos_x+blockwidth)) && (posx >= pos_x) 
            && (posy <= (pos_y+blockheight)) && (posy >= pos_y) )
            {
                active=false;
                draw(tft);
                // calculate new reflection angle
                if (movex<0)  // moving left
                {    if (posx <= (pos_x+blockwidth))
                    { result = 1; return result;  }
                }
                else
                {    if (posx >= pos_x)
                    { result = 2; return result;  }
                }

                if (movey<0)  // moving down
                {    if (posy <= (pos_y+blockheight))
                    { result = 3; return result;  }
                }
                else
                {    if (posy >= pos_y)
                    { result = 4; return result;  }
                }   
                return 0;  // should never happen! pass thru...             
            }       
    }

    return result;
}

