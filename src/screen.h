#ifndef SCREEN_HPP_
#define SCREEN_HPP_

//#include "Waveshare_ILI9486_GFX.h"


#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>

  //regular UNO shield on TTGO D1 R32 (ESP32)
#define LCD_RD  2  //LED
#define LCD_WR  4
#define LCD_RS 15  //hard-wired to A2 (GPIO35) 
#define LCD_CS 33  //hard-wired to A3 (GPIO34)
#define LCD_RST 32 //hard-wired to A4 (GPIO36)

#define LCD_D0 12
#define LCD_D1 13
#define LCD_D2 26
#define LCD_D3 25
#define LCD_D4 17
#define LCD_D5 16
#define LCD_D6 27
#define LCD_D7 14

#define ILI9486_YELLOW      0xFE00 // FFE0
#define ILI9486_YELLOW2     0xFF34
#define ILI9486_BLACK       0x0000 
#define ILI9486_CYAN        0x07FF 

class Screen
{
private:
    //Waveshare_ILI9486_GFX tft;
    MCUFRIEND_kbv tft;

public:
    Screen(void);
    void test(void);
    long getwidth(void);
    long getheight(void);
    void fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawText(String text, int16_t x, int16_t y);
    void setRotation(int16_t rot);
    void fillScreen(uint16_t color);
};

#endif // SCREEN_HPP_