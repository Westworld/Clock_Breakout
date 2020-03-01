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

void Block::setUnused(void) 
{
    used = false;
    active = false;
}

void Block::draw(Screen * tft)
{
    if (used) {
        if (active)
            tft->fillRect(pos_x, pos_y, blockwidth, blockheight, color);
        else
        {
            //tft->fillRect(pos_x, pos_y, blockwidth, blockheight, ILI9486_YELLOW2);
            tft->drawRect(pos_x, pos_y, blockwidth, blockheight, color);
            tft->drawRect(pos_x+1, pos_y+1, blockwidth-2, blockheight-2, color);
        }
            
    } 
}

void Block::undraw(Screen * tft)
{
    if (used) {
        tft->fillRect(pos_x, pos_y, blockwidth, blockheight, ILI9486_BLACK);  
    } 
}

bool Block::check(Ballsize ballsize, Ball * ball, Screen * tft) {
   bool result=false;
// 0/0 is right bottom
    if (used) {
        /*
                if ( (ballsize.x <= (pos_x+blockwidth)) && (ballsize.x2 >= pos_x) 
            && (ballsize.y <= (pos_y+blockheight)) && (ballsize.y2 >= pos_y) )
            */
        if ( (ballsize.x2 >= (pos_x)) && (ballsize.x <= (pos_x+blockwidth)) 
            && (ballsize.y2 >= (pos_y)) && (ballsize.y <= (pos_y+blockheight)) )
            {
                if (!active) 
                {
                    return true;
                }
                else
                {
                    //Console::info("block x [%d]: y [%d] ", ballsize.movex, ballsize.movey);

                    active=false;
                    undraw(tft);
                    //draw(tft);

                    // wenn er runtergeht und x und x2 außerhalb, dann noch entweder links oder rechts

                    if ((ballsize.y2 > (pos_y+1)) && (ballsize.y < (pos_y+blockheight+1)))
                        ball->InvertMove(true,false);
                    else
                        ball->InvertMove(false,true);
                    
                    return true;
                }    
            }       
    }

    return result;

}

int16_t Block::check(int16_t posx, int16_t posy, int16_t movex, int16_t movey, Screen * tft)
{
    int16_t result=false;

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
                return false;  // should never happen! pass thru...             
            }       
    }

    return false;
}

