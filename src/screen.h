#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#ifdef TARGET_esp32
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <TetrisMatrixDraw.h>

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
#endif

#ifdef TARGET_8266
#include <SPI.h>
#include <TetrisMatrixDraw.h>

//#define USER_SETUP_LOADED
/*
#define RPI_ILI9486_DRIVER
#define PIN_D5  14  // GPIO14       HSCLK
#define PIN_D6  12  // GPIO12       HMISO
#define PIN_D7  13  // GPIO13       HMOSI  RXD2
#define TFT_CS   PIN_D8  // GPIO15  Chip select control pin D8
#define TFT_DC   PIN_D3  // GPIO0   Data Command control pin
#define TFT_RST  PIN_D4  // GPIO2   Reset pin (could connect to NodeMCU RST, see next line)

#define RPI_ILI9486_DRIVER // 20MHz maximum SPI
*/

//#include <User_Setups/Setup5_RPi_ILI9486.h>   // für 4" Waveshare

// #include <User_Setups/Setup20_ILI9488.h>   // für 4" noname

#include <TFT_eSPI.h> // Hardware-specific library




       // Invoke custom library
#endif

#define ILI9486_YELLOW      0xFE00 // FFE0
#define ILI9486_YELLOW2     0xFF34
#define ILI9486_BLACK       0x0000 
#define ILI9486_CYAN        0x07FF 


class Screen
{
private:
    //Waveshare_ILI9486_GFX tft;
    #ifdef TARGET_esp32
    MCUFRIEND_kbv tft;
    #endif
    #ifdef TARGET_8266
    TFT_eSPI tft = TFT_eSPI();
    setup_t user;
    #endif
    TetrisMatrixDraw * tetris;

public:
    Screen(void);
    void test(void);
    long getwidth(void);
    long getheight(void);
    void fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t corner, uint16_t color);
    void drawText(String text, int16_t x, int16_t y);
    void setRotation(int16_t rot);
    void fillScreen(uint16_t color);
    void drawicon(int x, int y, const uint16_t *icon); 


    int8_t getPinName(int8_t pin);

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