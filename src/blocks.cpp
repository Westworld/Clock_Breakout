#include "Blocks.h"


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

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksx)+(j*blocksy);
            if (curelement < maxelements)
                allblocks[curelement]->activate(i*(blockwidth+5), j*(blockheight+5));
        }
    }
}

void Blocks::draw()
{
int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement;

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksx)+(j*blocksy);
            if (curelement < maxelements)
                allblocks[curelement]->draw(tft);
        }
    }
  
}