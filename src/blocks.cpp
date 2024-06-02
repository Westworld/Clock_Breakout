#include "Blocks.h"
#include "digits.h"

extern int16_t blockwidth, blockheight, blockstartx, blockstarty, blockoffset;

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
            curelement = (i*blocksy)+(j);
            if (curelement < maxelements)
                allblocks[curelement]->activate(i*(blockwidth+10), screenstarty- (j*(blockheight+10)), 1);
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
                allblocks[curelement++]->activate(x, y, 1);
    }
}


void Blocks::Setup(byte digit[4])
{
    InvaderShift = !InvaderShift;

    // start für position 1 und digit 1
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement=0;
   int16_t x, y, startx=screenstartx;

    for (int16_t digitcounter=0; digitcounter<maxelements; digitcounter++) {
        allblocks[digitcounter]->setUnused();
    }

    for (int16_t digitcounter=0; digitcounter<4; digitcounter++) {
        int16_t value = digit[digitcounter];
        if ((value < 0) || (value>9)) return;
        for (uint16_t i=0; i<sizeof(ziffern[value]); i+=2) {
            if (ziffern[value][i] != 10)
            {
                x = (ziffern[value][i]*((blockwidth/2)+2)) + startx;
                y = screenstarty - (ziffern[value][i+1]*((blockheight/2)+2));
                x = blockstartx-x;
                y = blockstarty-y;
                if (curelement < maxelements)
                    allblocks[curelement++]->activate(x, y, ziffern[value][i+1]);
            }
        }
        startx += (3*(blockwidth+10));
        if (digitcounter == 1) {
            //startx -= (blockwidth+10);
            x = ((blockwidth/2)+2) + startx;
            y = screenstarty - (2*((blockheight/2)+2));
            x = blockstartx-x;
            y = blockstarty-y;
            allblocks[curelement++]->activate(x, y, 1);

            x = ((blockwidth/2)+2) + startx;
            y = screenstarty - (6*((blockheight/2)+2));
            x = blockstartx-x;
            y = blockstarty-y;
            allblocks[curelement++]->activate(x, y, 1);
            startx += (2*(blockwidth+blockoffset));
        }
    }
}

void Blocks::setColor(int16_t thecolor)
{
int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement;

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksy)+(j);
            if (curelement < maxelements) {
                allblocks[curelement]->setColor(thecolor);
                }
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

void Blocks::draw(int16_t old_x, int16_t old_y, int16_t loop, int16_t move_x, int16_t move_y, bool Space_Shift)
{
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   if (loop >= maxelements) return;

   allblocks[loop]->undraw(tft, old_x, old_y);
   allblocks[loop]->draw(tft, old_x+move_x, old_y+move_y, Space_Shift);
}

void Blocks::draw(Ball * ball)  // only draw blocks close to the ball position
{
   int16_t maxelements = sizeof(allblocks)/sizeof(allblocks[0]);
   int16_t curelement;
   Ballsize thesize = ball->GetSize();
   thesize.x -=(blockwidth+5); thesize.x2 +=(blockwidth+5);
   thesize.y -= (blockheight+5); thesize.y2 +=(blockheight+5);

   for (int16_t i=0;i<blocksx;i++) {
        for (int16_t j=0; j<blocksy; j++) {
            curelement = (i*blocksy)+(j);
            if (curelement < maxelements) {
                allblocks[curelement]->draw(tft, thesize);
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
        if (allblocks[i]->used) {
            result = allblocks[i]->check(thesize, ball, tft);
            if (result)
            {   ball->SetBlock(i);
                return; 
            }            
        }
    }    
}

bool Blocks::checkShot(int16_t x, int16_t y) {
    bool result;
    for (int16_t i=0; i<numberblocks; i++) {
        if (allblocks[i]->used && allblocks[i]->active) {
            result = allblocks[i]->check(x, y, tft);
            if (result)
            {   
                return true; 
            }            
        }
    }  
    return false;    
}

void Blocks::findNearestBlock(int16_t &x, int16_t &y, int16_t paddle_x) {   
    int16_t blockx, blocky;
    x = 0; y = 10000;

    // find lowestBlock above paddle

    for (int16_t i=0; i<numberblocks; i++) {
        if ((allblocks[i]->used) && (allblocks[i]->active)) {
            if(allblocks[i]->isNearestBlock(paddle_x, blockx, blocky)) {
                if (blocky < y) {
                    y = blocky;
                    x = blockx;
                    return;
                }
            }
        }
    }    
}

int16_t Blocks::findNearestBlock(int16_t paddle_x) {   
    int16_t blockx, blocky, x = 0, test=0;

    // find nearest block above paddle

    for (int16_t i=0; i<numberblocks; i++) {
        if ((allblocks[i]->used) && (allblocks[i]->active)) {
            x += allblocks[i]->isNearestBlock(paddle_x);
            test++;
        }
    }    
    if ((test != 0) && (x == 0))
        return -2;  // we are in the middle, both side have same number of blocks
    return x;
}

void Blocks::ClearSurvivingBlocks(int16_t old_x, int16_t old_y) {
  // for space invader to start new game
    for (int16_t i=0; i<numberblocks; i++) {
        if ((allblocks[i]->used) && (allblocks[i]->active)) {
           // allblocks[i]->setColor(TFT_RED);
            allblocks[i]->drawendofgame(tft, TFT_RED);
            //allblocks[i]->draw(tft, old_x, old_y);  // wrong position
            delay(2);
        }
    }  
}