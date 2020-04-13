#ifdef TARGET_esp32
#include "WiFi.h"
#include <WiFiMulti.h>
#endif
#ifdef TARGET_8266

//#include <ESP8266mDNS.h>
#include "FS.h"
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif

#include <Arduino.h>

#include "main.h"
#include "screen.h"
#include "ball.h"
#include "paddle.h"
#include "blocks.h"
#include "shot.h"
//#include "ntp_time.h"

#include <Console.h>

#include <TetrisMatrixDraw.h>

//#include "../../../wifisetting.h"

// don't forget to set in Console.h debug = none

Screen * tft;
Ball * ball;
Paddle * paddle;
Blocks * blocks;

Shot * shotup;
Shot * shotdown;

short GameType = -1;  // 0 = Arkonid, 1 = Tetris

#ifdef TARGET_esp32
WiFiMulti wifiMulti;
#endif
#ifdef TARGET_8266
ESP8266WiFiMulti wifiMulti;
#endif

byte uhrzeit[6] = {1, 2, 3, 0, 0, 0};
  int16_t last_hour = -1;
  int16_t last_min  = -1;
  int16_t last_sec  = -1;

#define NTP_SERVER "de.pool.ntp.org"
#define TZ_INFO "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // Western European Time
const char* wifihostname = "Block Clock";
  int16_t curBlock;
int16_t loopcounter=0;

#define rotate 1

// Tetris
bool twelveHourFormat = true;
bool forceRefresh = true;


void setup() {
Serial.begin(115200);
//Serial.setDebugOutput(true);

   tft = new Screen();
   #ifdef rotate
   tft->setRotation(3); 
   #else
   tft->setRotation(1); 
   #endif
   tft->fillScreen(ILI9486_BLACK);
   tft->drawText("Start",0,0);
   delay(1);
   //tft->test();
   ConnectWifi();
       
   #ifdef TARGET_esp32
     struct tm local;
     configTzTime(TZ_INFO, NTP_SERVER); // ESP32 Systemzeit mit NTP Synchronisieren
     getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren
   #endif
   #ifdef TARGET_8266
     time_t now;
     struct tm * timeinfo;
     configTime(1 * 3600, 1 * 3600, "time.nist.gov", "time.windows.com", "de.pool.ntp.org");
     tft->drawText("waiting for time",0,30);
     while (!time(nullptr)) {
        Serial.print(".");
        delay(1000);
     }
     time(&now);
     timeinfo = localtime(&now); 
   #endif

   tft->drawText("got time",0,40);
   delay(2000);
   tft->fillScreen(ILI9486_BLACK);
   #ifdef rotate
   tft->setRotation(1); 
   #else
   tft->setRotation(3); 
   #endif
   
   ball = new Ball(tft);
   ball->setAngle(37);
   // winkel setzen, nicht 45°

   paddle = new Paddle(tft);
   blocks = new Blocks(tft);
   
   shotup = new Shot(tft, 8);
   shotdown = new Shot(tft, -8);

      //for (short i=0; i<3; i++)
   tetristest(); 

   CheckTime();
}



void ConnectWifi() {
 WiFi.mode(WIFI_STA);
    #ifdef TARGET_esp32
    WiFi.setHostname(wifihostname);
    #endif
    #ifdef TARGET_8266
    WiFi.hostname(wifihostname);
    #endif
    wifiMulti.addAP(WIFI_SSID, WIFI_PASS);
    wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2); 

    int loop=1;

    tft->drawText("Connecting Wifi...",0,1);

    while ((loop < 10) && (wifiMulti.run() != WL_CONNECTED)) {
      tft->drawText("try again to connect",0,20*loop);
      delay(1000);
    }
    if(loop < 10) {
         tft->drawText("connected",0,20);
    }
    else
    {
           tft->drawText("NOT CONNECTED ********+",0,20);
           tft->drawText(WIFI_SSID,0,40);
           tft->drawText(WIFI_SSID2,0,60);
           delay(5000);
    }
}

void GetTime( int16_t &hour, int16_t &min, int16_t &sec) {
#ifdef TARGET_esp32
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    tft->drawText("Failed to obtain time",0,0);
    timeinfo.tm_hour=12;
    timeinfo.tm_min=30;
  }
  hour = timeinfo.tm_hour;
  min  = timeinfo.tm_min;
  sec = timeinfo.tm_sec;
   #endif
   #ifdef TARGET_8266
     time_t now;
     struct tm * timeinfo;
     time(&now);
     timeinfo = localtime(&now); 
     hour = timeinfo->tm_hour;
     min  = timeinfo->tm_min;
     sec = timeinfo->tm_sec;
   #endif

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

    int16_t newgame = uhrzeit[2] % 2;  // 1
  
      GameType = newgame;
       switch (GameType) {
        case Arkonoid: 
          InitArkonid();
          break;
        case Tetris: 
          InitTetris();
          break;
        default:
          // nothing 
          ;
        }   
    last_hour = cur_hour;
    last_min = cur_min;
    last_sec = cur_sec;
  }
}

void InitArkonid() {
   #ifdef rotate
   tft->setRotation(1); 
   #else
   tft->setRotation(3); 
   #endif
  #ifdef TARGET_8266
   SetGame(Arkonoid, 4);
   #else
   SetGame(Arkonoid, 2);
   #endif

    tft->fillScreen(ILI9486_BLACK);
    paddle->draw();
    ball->SetY(20);
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
  while(!(tft->Tetris_drawNumbers(40,250, displaycolon, -1))) {
    delay(45);
    yield();
    GetTime();

    displaycolon = ((uhrzeit[5] % 2) == 1);
    //tft->Tetris_drawNumbers(40,250, displaycolon, ILI9486_BLACK);
  }
  //tft->Tetris_drawNumbers(40,250, displaycolon, -2);
}

void PlayTetris() {
  bool displaycolon = ((uhrzeit[5] % 2) == 1);
  tft->Tetris_drawNumbers(40,250, displaycolon, -1);
}

void loop() {

  if (++loopcounter>30) {
  CheckTime();
  loopcounter=0;
  }

  switch (GameType) {
     case Arkonoid: 
      PlayArkonid();
      break;
    case Tetris: 
      PlayTetris();
      break;    
    default:
      // nothing 
      ;
  }   

  delay(5);
  yield();
}


void tetristest() {
  tft->setRotation(1);
  tft->fillScreen(ILI9486_BLACK);

    #ifdef TARGET_8266
   SetGame(Space_Invader, 4);
   #else
   SetGame(Space_Invader, 2);
   #endif

   GetTime();

   paddle->draw();
   blocks->Setup(uhrzeit);
   shotup->deactivate();
   shotdown->deactivate();


  int16_t move_x = 0, move_y = 0;
  int16_t domovex = -5, domovey = 0;

  int16_t loopcounter = 0, loopx = 0, blockcounter = 0;
  int16_t maxxloop = 32 * numberblocks * 10;  // 24000
  while (loopcounter < maxxloop) {
    if (blockcounter <=numberblocks) {
      blocks->draw(move_x, move_y, blockcounter++, domovex, domovey);
    }
    else {
        blockcounter = 0;
        move_x += domovex;
        if (domovey != 0) {
          move_y -= 15;
          domovey = 0;
        } 

      if (++loopx >=32 ) {
          loopx = 0;
          domovey = -15;
          domovex = -domovex;
      }  
    }

    loopcounter++;

    if ((loopcounter % 15) == 1 ) {
      if (!shotup->move_draw()) {
          shotup->activate(paddle->getX(), 12);
        // new shot up, set x depending of paddle, which sets ative again      
      }


      if (!shotdown->move_draw()) {  // untested
          int16_t x, y;
          blocks->findNearestBlock(x, y, paddle->getX());
          if (x != 0)
            shotdown->activate(x, y);
      } 
    }


    if (blocks->checkShot(shotup->getX(), shotup->getY(true))) 
      {  // check hit on block ####################
            shotup->deactivate();
      }

    // move paddle to follow lowest block, avoid shotdown

    yield();
    delay(2);
  }

  // schuß stoppt wenn ganz oben oder treffer auf block

  delay(5000);
}