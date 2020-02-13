#ifndef BLOCKS_HPP_
#define BLOCKS_HPP_

#include "block.h"
#include "screen.h"


#define blocksx 14
#define blocksy 6
#define screenstartx 2
#define screenstarty 20

#define numberblocks 70

class Blocks
{
private:
    Block* allblocks[numberblocks];
    Screen * tft;




public:
    Blocks(Screen * mastertft);
    void Setup(void);
    //void update(float ballpos);
    void draw(void);
    void checkBall(Ball * ball);
    void drawBlock(int16_t curBlock);
};

#endif // BLOCKS_HPP_