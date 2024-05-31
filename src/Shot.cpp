#include "Shot.h"

Shot::Shot(Screen * mastertft, int16_t movey)
{
    tft = mastertft;
    pos_x = tft->getwidth() / 2;
    pos_y = 1;   
    move_y = movey;
    max_x = tft->getwidth();
    max_y = tft->getheight();
}

Shotsize Shot::GetSize(void) {
    Shotsize thesize;

    thesize.x = pos_x; thesize.x2 = pos_x+shotwidth;
    thesize.y = pos_y; thesize.y2 = pos_y+shotheight;
    
    thesize.movey = move_y;

    return thesize;
}

int16_t Shot::getX(void) {
    if (active) {
        if ((pos_y>0) && (pos_y < max_y))
            return pos_x;
        else
        {
            return 0;
        }
            
    }    
    else
    {
            return -1;
    }
    
}

int16_t Shot::getY(bool up) {
    if (active) {
        if (up)
            return pos_y+shotheight;
        else
            return pos_y;
    }
    else
        return -1;        
    
}

void Shot::activate(int16_t posx, int16_t posy, bool down) {
    pos_x = posx;
    pos_y = posy;
    if (down) {
        pos_y -= shotheight;
        color = TFT_WHITE;
    }
    else
    {
            color = ILI9486_YELLOW;
    }
        
    active = true;
}

void Shot::draw(void) {
    if (active) {
        int16_t x, y;
        x = (int16_t) pos_x;
        y = (int16_t) pos_y;    
        tft->fillRect(x, y, shotwidth, shotheight, ILI9486_YELLOW); //color);
    }
}

void Shot::undraw(void){
    int16_t x, y;
    x = (int16_t) pos_x;
    y = (int16_t) pos_y; 
    tft->fillRect(x, y, shotwidth, shotheight, ILI9486_BLACK);
}

int16_t Shot::move_draw(void) {
    if (active) {
        this->undraw();
        pos_y = pos_y + move_y;
        if ((pos_y < -150) || (pos_y > max_y))
            active = false;
        this->draw();  
    }
    return active; //blockid; 
}

void Shot::deactivate(void) {
    active = false;
    undraw();
}