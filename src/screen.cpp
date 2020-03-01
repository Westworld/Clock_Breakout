#include "Screen.h"
//#include "LCD_Driver.h"


Screen::Screen() {
    // tft = Waveshare_ILI9486_GFX();
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID
    tft.begin(ID);

    tft.setRotation(3);
    //LCD_SetArealColor( 0, 0, 300, 300, 0xFFFF);
    tft.fillScreen(ILI9486_BLACK);
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
void Screen::fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color) {
    tft.fillCircle(x, y, radius, color);
}
void Screen::fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color) {
    tft.fillRect(x, y, radius, radius, color);
}    

void Screen::fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    tft.fillRect(x, y, width, height, color);
}

void Screen::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    tft.drawRect(x, y, width, height, color);
}