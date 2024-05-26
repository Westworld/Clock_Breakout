
#include "Screen.h"


Screen::Screen() {
    tft.init();

    x_offset=0;y_offset=0;

    tetris = new TetrisMatrixDraw(tft);

    tft.setRotation(3);
    tft.fillScreen(ILI9486_BLACK);
}

void Screen::setOffset(int16_t x, int16_t y) {
    x_offset=x;
    y_offset=y;
}

void Screen::reset() {
    x_offset=0;y_offset=0;
    tft.fillScreen(ILI9486_BLACK);
}

void Screen::setRotation(int16_t rot) {
    tft.setRotation(rot);
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
    tft.fillCircle(x + x_offset, y + y_offset, radius, color);
}
void Screen::fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color) {
    tft.fillRect(x + x_offset, y + y_offset, radius, radius, color);
}    

void Screen::fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    tft.fillRect(x + x_offset, y + y_offset, width, height, color);
}

void Screen::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    tft.drawRect(x + x_offset, y + y_offset, width, height, color);
}

void Screen::drawRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t corner, uint16_t color) {
    tft.drawRoundRect(x + x_offset, y + y_offset, width, height, corner, color);
}

void Screen::setTextColor(int16_t front,int16_t back) {
    tft.setTextColor(front,back); 
}

void Screen::setTextSize(int16_t size) {
    tft.setTextSize(size);
}

 void Screen::setFreeFontFix() {
     tft.setFreeFont(FF20); // FF20
 }

 void Screen::clearFreeFont() {
     tft.setFreeFont(NULL); // FF20
 }

 void Screen::drawNumber(int16_t number, int16_t x, int16_t y) {
     tft.drawNumber(number, x + x_offset, y + y_offset);
 }

void Screen::drawText(String text, int16_t x, int16_t y) {
        tft.setTextColor(ILI9486_YELLOW, ILI9486_BLACK);
        //tft.setCursor(x + x_offset, y + y_offset);
        tft.drawString(text, x + x_offset, y + y_offset, 2);
        //tft.println(text);
        
}

void Screen::fillScreen(uint16_t color) {
    tft.fillScreen(color);
}

void Screen::drawicon(int x, int y, const uint16_t *icon) { // Draws the graphics icons based on stored image bitmaps  

 tft.pushImage(x + x_offset, y + y_offset, 28, 28, icon);
  
}


void Screen::Tetris_setText(String txt, bool forceRefresh) {
    tetris->setText(txt, forceRefresh);
}

bool Screen::Tetris_drawText(int x, int y) {
    return tetris->drawText(x, y);
}    

bool Screen::Tetris_drawText(int x, int y, int color) {
    return tetris->drawText(x, y, color);
}  

 void Screen::Tetris_DrawChar(String letter, uint8_t x, uint8_t y, uint16_t color) {
     tetris->drawChar(letter,  x,  y,  color);
 }

 void Screen::Tetris_drawShape(int blocktype, uint16_t color, int x_pos, int y_pos, int num_rot) {
     tetris->drawLargerShape(10, blocktype,  color,  x_pos,  y_pos,  num_rot);
 }

void Screen::Tetris_setNumbers(long nummer) {
     tetris->setNumbers(nummer);
 }

bool Screen::Tetris_drawNumbers(int x_pos, int y_pos, bool drawColon){
     return tetris->drawNumbers(x_pos,y_pos, drawColon);
 }

bool Screen::Tetris_drawNumbers(int x_pos, int y_pos, bool drawColon, int color) {
     return tetris->drawNumbers(x_pos,y_pos, drawColon, color);
 }

void Screen::Tetris_setTime(char * timeString) {
    tetris->setTime(timeString, true);
}

void Screen::tft_setSwapBytes(bool swap) {
    tft.setSwapBytes(swap);
}

 // This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool Screen::tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}