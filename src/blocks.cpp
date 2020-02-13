#include "Blocks.h"

#include <Console.h>
#include <digits.h>

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

void Blocks::Setup(int16_t position, int16_t digit)
{
    // start für position 1 und digit 1
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement=0;
   int16_t x, y;

    for (uint16_t i=0; i<sizeof(value1); i+=2) {
            x = (value1[i]*blockwidth) + screenstartx;
            y = screenstarty - (value1[i+1]*blockheight);
            if (curelement < maxelements)
                allblocks[curelement++]->activate(x, y);
    }
}


void Blocks::Setup(byte digit[4])
{
    // start für position 1 und digit 1
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement=0;
   int16_t x, y, startx=screenstartx;


    for (int16_t digitcounter=0; digitcounter<4; digitcounter++) {
        int16_t value = digit[digitcounter];
        if ((value < 0) || (value>9)) return;

        for (uint16_t i=0; i<sizeof(ziffern[value]); i+=2) {
            x = (ziffern[value][i]*(blockwidth+10)) + startx;
            y = screenstarty - (ziffern[value][i+1]*(blockheight+10));
            if (curelement < maxelements)
                allblocks[curelement++]->activate(x, y);
        }
        startx += (3*(blockwidth+10));
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

void Blocks::drawBlock(int16_t curBlock)
{
    int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);

    if ((curBlock >=0) && (curBlock < maxelements))
        allblocks[curBlock]->draw(tft);
}

void Blocks::checkBall(Ball * ball)
{
    Ballsize thesize = ball->GetSize();
    int16_t result;

    ball->SetBlock(-1);
    for (int16_t i=0; i<numberblocks; i++) {
        if (allblocks[i]->active) {
            result = allblocks[i]->check(thesize, ball, tft);
            if (result)
            {   ball->SetBlock(i);
                return; 
            }            
        }
    }    
}