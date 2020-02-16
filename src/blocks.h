#ifndef BLOCKS_HPP_
#define BLOCKS_HPP_

#include "block.h"
#include "screen.h"


#define blocksx 12
#define blocksy 5
#define screenstartx 2
#define screenstarty 280

#define numberblocks 75

class Blocks
{
private:
    Block* allblocks[numberblocks];
    Screen * tft;




public:
    Blocks(Screen * mastertft);
    void Setup(void);
    void Setup(int16_t position, int16_t digit);
    void Setup(byte digit[4]);
    //void update(float ballpos);
    void draw(void);
    void checkBall(Ball * ball);
    //void checkBall(Ball * ball);  ballsize
    void drawBlock(int16_t curBlock);

};

#endif // BLOCKS_HPP_