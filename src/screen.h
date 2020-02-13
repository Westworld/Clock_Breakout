#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include "Waveshare_ILI9486_GFX.h"


class Screen
{
private:
    Waveshare_ILI9486_GFX tft;

public:
    Screen(void);
    void test(void);
    long getwidth(void);
    long getheight(void);
    void fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
};

#endif // SCREEN_HPP_