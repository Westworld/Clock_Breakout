#include "Blocks.h"

#include <Console.h>


Blocks::Blocks(Screen * mastertft)
{
    tft = mastertft;

    for (int16_t i=0; i<numberblocks; i++) {
        allblocks[i] = new Block();
    }
}

void Blocks::Setup(void)
{
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement;

   //Console::info("Blocks setup");

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksy)+(j);
            //Console::info("element [%d]: ", curelement);
            if (curelement < maxelements)
                allblocks[curelement]->activate(i*(blockwidth+10), 300- (j*(blockheight+10)));
        }
    }
}

void Blocks::draw()
{
int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement;

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksy)+(j);
            if (curelement < maxelements) {
                allblocks[curelement]->draw(tft);
                }
            }
    }
  
}

bool Blocks::check(int16_t pos_x, int16_t pos_y, int16_t move_x, int16_t move_y)
{
     bool result=false;
     for (int16_t i=0; i<numberblocks; i++) {
                if (allblocks[i]->active) {
                    result = allblocks[i]->check(pos_x, pos_y, move_x, move_y, tft);
                    if (result)
                         break;
                    }
            }
    return result;      
}