#include "Screen.h"
#include "LCD_Driver.h"


Screen::Screen() {
    tft = Waveshare_ILI9486_GFX();
    tft.begin();
    tft.setRotation(3);
    //LCD_SetArealColor( 0, 0, 300, 300, 0xFFFF);
    //tft.fillScreen(ILI9486_BLACK);
}

void Screen::test(void) {
    int 
    cx = tft.width(),
    cy = tft.height();

    tft.drawRect(0,0 , cx, cy, ILI9486_CYAN); 
}

long Screen::getwidth(void) { 
    return tft.width();
 }
long Screen::getheight(void){ 
    return tft.height();
}
void Screen::fillCircle(long x, long y, long radius, long color) {
    tft.fillCircle(x, y, radius, color);
}
void Screen::fillRect(long x, long y, long radius, long color) {
    tft.fillRect(x, y, radius, radius, color);
    //LCD_SetArealColor( x, y, x+radius, y+radius, color);
}

