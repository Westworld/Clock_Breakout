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
    void fillCircle(long x, long y, long radius, long color);
    void fillRect(long x, long y, long radius, long color);
};

#endif // SCREEN_HPP_