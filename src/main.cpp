#include "WiFi.h"
#include <WiFiMulti.h>

// Include SPIFFS
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" 
#include <Arduino.h>

#include "main.h"
#include "screen.h"
#include "ball.h"
#include "paddle.h"
#include "blocks.h"
#include "shot.h"
#include "Pacman.h"

#include <TetrisMatrixDraw.h>
#include <TJpg_Decoder.h>

#include <time.h>  

#define CYD_LED_BLUE 17
#define CYD_LED_RED 4
#define CYD_LED_GREEN 16

Screen * tft;
Ball * ball;
Paddle * paddle;
Blocks * blocks;

Shot * shotup;
Shot * shotdown;

short GameType = 1; // first 10 minutes Pacman  // 0 = Arkonid, 1 = Tetris
   #define TetrisXStart 8
   #define TetrisYStart 450

WiFiMulti wifiMulti;

byte uhrzeit[6] = {1, 2, 3, 0, 0, 0};
  int16_t last_hour = -1;
  int16_t last_min  = -1;
  int16_t last_sec  = -1;

#define NTP_SERVER "de.pool.ntp.org"
//#define TZ_INFO "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03" 
const char* wifihostname = "Block Clock";
  int16_t curBlock;
int16_t loopcounter=0;


// Tetris
bool twelveHourFormat = true;
bool forceRefresh = true;


// Invaders
  int16_t invaders_move_x, invaders_move_y;
  int16_t invaders_domovex, invaders_domovey;
  int16_t invaders_loopx, invaders_blockcounter;
  int32_t invaders_maxxloop, invaders_loopcounter;


void setup() {
Serial.begin(115200);

  pinMode(CYD_LED_RED, OUTPUT);  // all off
  pinMode(CYD_LED_GREEN, OUTPUT);
  pinMode(CYD_LED_BLUE, OUTPUT);
  digitalWrite(CYD_LED_RED, HIGH); 
  digitalWrite(CYD_LED_GREEN, HIGH);
  digitalWrite(CYD_LED_BLUE, HIGH);

   tft = new Screen();
   #ifdef rotate
   tft->setRotation(3); 
   #else
   tft->setRotation(1); 
   #endif
   tft->fillScreen(ILI9341_BLACK);
   tft->setTextSize(2);

   tft->drawText("Start",00,00);
   Serial.println("before Wifi");
 
   //tft->test();
   ConnectWifi();
    Serial.println("after Wifi");
     struct tm local;
     configTzTime(MY_TZ, NTP_SERVER); // ESP32 Systemzeit mit NTP Synchronisieren
     getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren

   tft->drawText("got time        ",0,60);

   // Initialise SPIFFS
  if (!SPIFFS.begin()) {
    tft->drawText("SPIFFS ERROR",0,90);
  }
  else
    tft->drawText("SPIFFS ready",0,90);

   delay(2000);
   tft->fillScreen(ILI9486_BLACK);
   #ifdef rotate
   tft->setRotation(1); 
   #else
   tft->setRotation(3); 
   #endif

    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);
  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
   
   ball = new Ball(tft);
   ball->setAngle(37);
   // winkel setzen, nicht 45°

   paddle = new Paddle(tft);
   blocks = new Blocks(tft);
   
   shotup = new Shot(tft, 8);
   shotdown = new Shot(tft, -8);

   //GetTime();
   Serial.println("startup before checktime");
   CheckTime();  // starts game
   Serial.println("startup after checktime");
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  return tft->tft_output(x, y, w, h, bitmap);

}

void ConnectWifi() {
 WiFi.mode(WIFI_STA);
    WiFi.setHostname(wifihostname);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
    wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2); 

    int loop=1;

    tft->drawText("Connecting Wifi...",0,30);

    while ((loop < 10) && (wifiMulti.run() != WL_CONNECTED)) {
      tft->drawText("try again to connect",0,30*loop);
      delay(1000);
    }
    if(loop < 10) {
         tft->drawText("connected           ",0,30);
    }
    else
    {
           tft->drawText("NOT CONNECTED ********+",0,30);
           tft->drawText(WIFI_SSID,0,60);
           tft->drawText(WIFI_SSID2,0,90);
           delay(5000);
    }
}

void GetTime( int16_t &hour, int16_t &min, int16_t &sec) {

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    tft->drawText("Failed to obtain time",0,0);
    timeinfo.tm_hour=12;
    timeinfo.tm_min=30;
  }
  hour = timeinfo.tm_hour;
  min  = timeinfo.tm_min;
  sec = timeinfo.tm_sec;

    uhrzeit[0] = hour / 10;
    uhrzeit[1] = hour % 10;
    uhrzeit[2] = min / 10;
    uhrzeit[3] = min % 10;
    uhrzeit[4] = sec / 10;
    uhrzeit[5] = sec % 10;
}

void GetTime() {
  int16_t cur_hour;
  int16_t cur_min;
  int16_t cur_sec;
  GetTime( cur_hour,cur_min, cur_sec);
}

void CheckTime() {
  int16_t cur_hour;
  int16_t cur_min;
  int16_t cur_sec;
  GetTime( cur_hour,cur_min, cur_sec);

  if ((cur_hour != last_hour) || (cur_min != last_min)) {
    int16_t newgame = uhrzeit[2] % 6;  
    //newgame=2;  // for testing
      
    if (newgame >= 5)
        newgame =  random(4);  // new game between 0-4

    switch (newgame) {
        case Arkonoid:  // 0
          GameType = Arkonoid;
          InitArkonid();
          break;
        case Tetris: 
          GameType = Tetris;
          InitTetris();
          break;
        case Space_Invader: 
          if (GameType == Space_Invader)
            ResetInvaders();
          else  
            GameType = Space_Invader;
          InitInvaders();
          break;    
        case Pict_Clock: 
          GameType = Pict_Clock;
          InitClock();
          break;
        case Pacman:  
          GameType = Pacman;
          pacman_init(tft);
          break;
                     
        default:
          GameType = Pacman;  // random?
          pacman_init(tft);
          break;    
        }  
   
    last_hour = cur_hour;
    last_min = cur_min;
    last_sec = cur_sec;

  }
}

void InitArkonid() {
   tft->reset();
   #ifdef rotate
   tft->setRotation(1); 
   #else
   tft->setRotation(3); 
   #endif

    SetGame(Arkonoid, 2);  // screen size

    tft->fillScreen(ILI9486_BLACK);
    blocks->setColor(ILI9486_YELLOW);
    paddle->setType(true);
    paddle->draw();
    ball->SetY(tft->getheight()-10);
    blocks->Setup(uhrzeit);
    blocks->draw();
}

void PlayArkonid() {
  curBlock = ball->move_draw();  
  blocks->checkBall(ball);
   
  paddle->update(ball->GetX());
  blocks->draw(ball);
}

void InitTetris() {

   tft->reset();
   #ifdef rotate
   tft->setRotation(3); 
   #else
   tft->setRotation(1); 
   #endif
  tft->fillScreen(ILI9486_BLACK);

  char timeString [8];
  sprintf(timeString, "%d%d:%d%d", uhrzeit[0], uhrzeit[1], uhrzeit[2], uhrzeit[3]);
  tft->Tetris_setTime(timeString);

  bool displaycolon = false;
  while(!(tft->Tetris_drawNumbers(TetrisXStart,TetrisYStart, displaycolon, -1))) {
    delay(60);
    yield();
    GetTime();

    displaycolon = ((uhrzeit[5] % 2) == 1);
  }
}

void PlayTetris() {
  bool displaycolon = ((uhrzeit[5] % 2) == 1);
  tft->Tetris_drawNumbers(TetrisXStart,TetrisYStart, displaycolon, -1);
}

void InitInvaders() {
  tft->reset();
  #ifdef rotate
   tft->setRotation(1); 
   #else
   tft->setRotation(3); 
   #endif

  tft->fillScreen(ILI9486_BLACK);

   SetGame(Space_Invader, 2);

   GetTime();
   blocks->setColor(TFT_WHITE);
   paddle->setType(false);
   paddle->setX(tft->getwidth()/2);
   paddle->draw();
   blocks->Setup(uhrzeit);
   shotup->deactivate();
   shotdown->deactivate();

  invaders_move_x = 0, invaders_move_y = 0;
  invaders_domovex = -3, invaders_domovey = 0;

  invaders_loopcounter = 0, invaders_loopx = 0, invaders_blockcounter = 0;
  invaders_maxxloop = 30 * numberblocks * 15;  // 24000
}

void ResetInvaders() {
   // new game, stop old. Mark all surviving blocks red!
  blocks->ClearSurvivingBlocks(invaders_move_x, invaders_move_y);
  delay(1000);
}

void PlayInvaders() {
static bool Inv_Shift=false;

Inv_Shift = !Inv_Shift;

if (invaders_loopcounter < invaders_maxxloop) {
    if (invaders_blockcounter <=numberblocks) {
      blocks->draw(invaders_move_x, invaders_move_y, invaders_blockcounter++, invaders_domovex, invaders_domovey, Inv_Shift);
    }
    else {
        invaders_blockcounter = 0;
        invaders_move_x += invaders_domovex;
        if (invaders_domovey != 0) {
          invaders_move_y -= 5;
          invaders_domovey = 0;
        } 

      if (++invaders_loopx >=20 ) {   // 31
          invaders_loopx = 0;
          invaders_domovey = -5;
          invaders_domovex = -invaders_domovex;
      }  
    }

    invaders_loopcounter++;

    if ((invaders_loopcounter % 15) == 1 ) {
      if (!shotup->move_draw()) {
          shotup->activate(paddle->getX()-8, 10, false);
        // new shot up, set x depending of paddle, which sets ative again      
      }


      if (!shotdown->move_draw()) {  
          int16_t x, y;
          blocks->findNearestBlock(x, y, paddle->getX());
          if (x != 0)
            shotdown->activate(x, y, true);
      } 

          // move paddle to follow lowest block, avoid shotdown
          // first check if shot is coming down, if yes move left/right (opposite of shot)
      int16_t x, paddlex;    
      x=shotdown->getX();
      paddlex = paddle->getX();
      if (x > 0)  {
        //paddle->undraw();
        if (x>paddlex)  {
          paddle->move(-1);
        }  
        else {
          paddle->move(1);
        }  
        //paddle->draw();  
      }
      else {
        // no risk being hit, but do we have a block above us or do we need to move?
        paddlex = paddle->getX();
        x = blocks->findNearestBlock(paddlex);      
 
        if (x != 0) {  
          //paddle->undraw();  
          if (x < 0)  
            paddle->move(-2);
          else
            paddle->move(2);
          //paddle->draw(); 
        }  
      }
    }

    if (blocks->checkShot(shotup->getX(), shotup->getY(true))) 
      {  // check hit on block 
            shotup->deactivate();
      }

    yield();
    //delay(2);
  }
}

void InitClock() {
  tft->reset();
  #ifdef rotate
   tft->setRotation(3); 
   #else
   tft->setRotation(1); 
   #endif
  tft->fillScreen(ILI9486_BLACK);
  tft->tft_setSwapBytes(true);
  GetTime();
}

void PlayClock() {
    // Get the width and height in pixels of the jpeg if you wish
  uint16_t w = 0, h = 0;
  
  static byte altMinute=-1;

  if (altMinute != uhrzeit[3]) {
     altMinute = uhrzeit[3];
 
      char filename[] = "/0/1.jpg";
      int16_t minute = random(0,10); //uhrzeit[3] % 10; 
      switch (minute) {
            case 0:  // 0
              filename[1] = '0';
              break;
            case 1:  // 0
              filename[1] = '1';
              break;
            case 2:  // 0
              filename[1] = '2';
              break;
            case 3:  // 0
              filename[1] = '3';
              break;
            case 4:  // 0
              filename[1] = '4';
              break;
            case 5: 
              filename[1] = '5';
              break; 
            case 6: 
              filename[1] = '6';
              break; 
            case 7: 
              filename[1] = '7';
              break; 
            case 8: 
              filename[1] = '8';
              break;                    
            case 9: 
              filename[1] = '9';
              break;  
            default:
            ;
      }        

    TJpgDec.getFsJpgSize(&w, &h, filename); // Note name preceded with "/"
      // vertikal in der Mitte
      //Nach Uhrzeit, Breite berücksichtigen
      uint16_t posy = (tft->getheight()/2)-(h/2);
      if(minute>=4) w+=5;

      filename[3] = 48+uhrzeit[0];
      Serial.print(filename);
      TJpgDec.drawFsJpg(0, posy, filename);

      filename[3] = 48+uhrzeit[1];
      TJpgDec.drawFsJpg(w, posy, filename);

        filename[3] = 48+uhrzeit[2];
      TJpgDec.drawFsJpg(w+w, posy, filename);

        filename[3] = 48+uhrzeit[3];
      TJpgDec.drawFsJpg(w+w+w, posy, filename);
  }
}


void loop() {

  if (++loopcounter>30) {
  CheckTime();
  loopcounter=0;
  }

  if (GameType < 0) {
      pacman_run(); 
  }
  else {

  switch (GameType) {
     case Arkonoid: 
      PlayArkonid();
      delay(5);
      break;
    case Tetris: 
      PlayTetris();
      delay(5);
      break;    
    case Space_Invader: 
      PlayInvaders();
      delay(2);
      break; 
    case Pict_Clock: 
      PlayClock();
      delay(100);
      break;  
    case Pacman:
      pacman_run(); // delay inside  
    default:
      pacman_run(); // delay inside  
      ;
  }   
  }
  yield();
}


void tetristest() {
   #ifdef rotate
   tft->setRotation(3); 
   #else
   tft->setRotation(1); 
   #endif
  tft->fillScreen(ILI9486_BLACK);
  tft->tft_setSwapBytes(true);
  GetTime();

    // Get the width and height in pixels of the jpeg if you wish
  uint16_t w = 0, h = 0;
  TJpgDec.getFsJpgSize(&w, &h, "/1/1.jpg"); // Note name preceded with "/"
  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0

  // vertikal in der Mitte
  //Nach Uhrzeit, Breite berücksichtigen
  uint16_t posy = (tft->getheight()/2)-(h/2);
  char filename[] = "/1/1.jpg";
  filename[3] = 48+uhrzeit[0];
  Serial.print(filename);
  TJpgDec.drawFsJpg(0, posy, filename);

  filename[3] = 48+uhrzeit[1];
  TJpgDec.drawFsJpg(w, posy, filename);

    filename[3] = 48+uhrzeit[2];
  TJpgDec.drawFsJpg(w+w, posy, filename);

    filename[3] = 48+uhrzeit[3];
  TJpgDec.drawFsJpg(w+w+w, posy, filename);

    yield();
    delay(10000);
tft->fillScreen(TFT_RED);
}