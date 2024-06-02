#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "screen.h"
#include "ball.h"

enum { Arkonoid, Tetris, Space_Invader, Pict_Clock, Pacman };

//alien 1 
PROGMEM const unsigned char a1a[] = {
8, 8, //pictureresolution
  0x18,0x7E,0xFF,0xDB,0xFF,0x66,0xDB,0x42
};

//alien 1 
PROGMEM const unsigned char a1b[] = {
8, 8, //pictureresolution
  0x18,0x7E,0xFF,0xDB,0xFF,0x24,0x5A,0x81
};

//alien 2 
PROGMEM const unsigned char a2a[] = {
8, 8, //pictureresolution
  0x42,0x24,0x7E,0xDB,0xFF,0xBD,0xA5,0x24
};

//alien 2
PROGMEM const unsigned char a2b[] = {
8, 8, //pictureresolution
  0x42,0x24,0xBD,0xDB,0xFF,0x7E,0x24,0x42
};

//alien 3
PROGMEM const unsigned char a3a[] = {
8, 8, //pictureresolution
  0x18,0x3C,0x7E,0x5A,0x7E,0x24,0x42,0x24
};

//alien 3
PROGMEM const unsigned char a3b[] = {
8, 8, //pictureresolution
  0x18,0x3C,0x7E,0x5A,0x7E,0x24,0x5A,0xA5
};


class Block
{
private:
    int16_t pos_x, pos_y;
    int16_t last_pos_x=0, last_pos_y=0;
    uint16_t color = ILI9486_YELLOW;
    int8_t row = 0;
    bool Invader_Shift=false;

public:
    bool active=false;
    bool used=false;

    Block(void);
    void activate(int16_t posx, int16_t posy, int8_t row) ;
    void setUnused(void);
    //void update(float ballpos);;
    void draw(Screen * mastertft);
    void drawendofgame(Screen * mastertft, uint16_t newcolor);
    void draw(Screen * mastertft, Ballsize ballsize);
    void draw(Screen * mastertft, int16_t move_x, int16_t move_y, bool Space_Shift);
    void draw(Screen * tft, int16_t move_x, int16_t move_y, int16_t othercolor);
    void undraw(Screen * mastertft);
    void undraw(Screen * mastertft, int16_t move_x, int16_t move_y);
    int16_t check(int16_t posx, int16_t pos_y, int16_t move_x, int16_t move_y, Screen * mastertft);
    bool check(int16_t posx, int16_t posy, Screen * tft);
    bool check(Ballsize ballsize, Ball * ball, Screen * tft);
    bool isNearestBlock(int16_t paddle_x, int16_t &blockx, int16_t &blocky);
    int16_t isNearestBlock(int16_t paddle_x);
    void setColor(int16_t othercolor);
    const unsigned char * getInvader(bool Space_Shift);
};

void SetGame(int16_t gameType, int16_t screenSize);

#endif // BLOCK_HPP_