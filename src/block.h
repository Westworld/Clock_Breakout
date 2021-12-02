#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "screen.h"
#include "ball.h"

/*
 #ifdef TARGET_8266
//#define blockwidth 23
#define blockheight 20
#define blockstartx 430
#define blockstarty 420
#define blockoffset 10
#endif

#ifdef TARGET_esp32
#define blockwidth 12
#define blockheight 14
#define blockstartx 280
#define blockstarty 400
#define blockoffset 10
#endif
*/

enum { Arkonoid, Tetris, Space_Invader, Pict_Clock, Pacman };



class Block
{
private:
    int16_t pos_x, pos_y;
    int16_t last_pos_x=0, last_pos_y=0;
    uint16_t color = ILI9486_YELLOW;

public:
    bool active=false;
    bool used=false;

    Block(void);
    void activate(int16_t posx, int16_t posy) ;
    void setUnused(void);
    //void update(float ballpos);;
    void draw(Screen * mastertft);
    void drawendofgame(Screen * mastertft, uint16_t newcolor);
    void draw(Screen * mastertft, Ballsize ballsize);
    void draw(Screen * mastertft, int16_t move_x, int16_t move_y);
    void draw(Screen * tft, int16_t move_x, int16_t move_y, int16_t othercolor);
    void undraw(Screen * mastertft);
    void undraw(Screen * mastertft, int16_t move_x, int16_t move_y);
    int16_t check(int16_t posx, int16_t pos_y, int16_t move_x, int16_t move_y, Screen * mastertft);
    bool check(int16_t posx, int16_t posy, Screen * tft);
    bool check(Ballsize ballsize, Ball * ball, Screen * tft);
    bool isNearestBlock(int16_t paddle_x, int16_t &blockx, int16_t &blocky);
    int16_t isNearestBlock(int16_t paddle_x);
    void setColor(int16_t othercolor);
};

void SetGame(int16_t gameType, int16_t screenSize);

#endif // BLOCK_HPP_