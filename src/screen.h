#ifndef SCREEN_HPP_
#define SCREEN_HPP_
#define TOUCH_CS // This sketch does not use touch, but this is defined to quiet the warning about not defining touch_cs.
#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Free_Fonts.h"
#include <TetrisMatrixDraw.h>

#define ILI9486_YELLOW      0xFE00 // FFE0
#define ILI9486_YELLOW2     0xFF34
#define ILI9486_BLACK       0x0000 
#define ILI9486_CYAN        0x07FF 


class Screen
{
private:
    TFT_eSPI tft = TFT_eSPI();
    setup_t user;

    TetrisMatrixDraw * tetris;
    int16_t x_offset, y_offset;

public:
    Screen(void);
    void setOffset(int16_t x, int16_t y);
    void reset(void);
    void test(void);
    long getwidth(void);
    long getheight(void);
    void fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t corner, uint16_t color);
    void drawText(String text, int16_t x, int16_t y);
    void drawNumber(int16_t number, int16_t x, int16_t y);
    void setTextColor(int16_t front,int16_t back);
    void setTextSize(int16_t size);
    void setFreeFontFix();
    void clearFreeFont();
    void setRotation(int16_t rot);
    void fillScreen(uint16_t color);
    void drawicon(int x, int y, const uint16_t *icon); 

    void Tetris_setText(String txt, bool forceRefresh);
    bool Tetris_drawText(int x, int y);
    bool Tetris_drawText(int x, int y, int color);
    void Tetris_setNumbers(long nummer);
    void Tetris_setTime(char * timeString);
    bool Tetris_drawNumbers(int x_pos, int y_pos, bool drawColon);
    bool Tetris_drawNumbers(int x_pos, int y_pos, bool drawColon, int color);
    void Tetris_DrawChar(String letter, uint8_t x, uint8_t y, uint16_t color);
    void Tetris_drawShape(int blocktype, uint16_t color, int x_pos, int y_pos, int num_rot);

    bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
    void tft_setSwapBytes(bool swap);
};

#endif // SCREEN_HPP_