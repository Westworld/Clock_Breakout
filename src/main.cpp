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
//#include "ntp_time.h"

#include <Console.h>

#include <TetrisMatrixDraw.h>

//#include "../../../wifisetting.h"

// don't forget to set in Console.h debug = none

Screen * tft;
Ball * ball;
Paddle * paddle;
Blocks * blocks;

#ifdef TARGET_esp32
WiFiMulti wifiMulti;
#endif
#ifdef TARGET_8266
ESP8266WiFiMulti wifiMulti;
#endif

byte uhrzeit[4] = {1, 2, 3, 0};
  int16_t last_hour = -1;
  int16_t last_min  = -1;

#define NTP_SERVER "de.pool.ntp.org"
#define TZ_INFO "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // Western European Time
const char* wifihostname = "Block Clock";
  int16_t curBlock;
int16_t loopcounter=0;


// Tetris
bool twelveHourFormat = true;
bool forceRefresh = true;


void setup() {
//Serial.begin(115200);
//Serial.setDebugOutput(true);

   tft = new Screen();
   tft->setRotation(1); 
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
     configTime(1 * 3600, 0, "time.nist.gov", "time.windows.com", "de.pool.ntp.org");
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
   tft->setRotation(3);

   for (short i=0; i<10; i++)
    tetristest(); 
   

    tft->setRotation(3);
    tft->fillScreen(ILI9486_BLACK);
    ball = new Ball(tft);
   ball->setAngle(37);
   // winkel setzen, nicht 45°
   // nicht komplett löschen und neu zeichnen, nur untere pixel?

   paddle = new Paddle(tft);
   paddle->draw();

   blocks = new Blocks(tft);
   
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
}



void CheckTime() {
  int16_t cur_hour;
  int16_t cur_min;
  int16_t cur_sec;
  GetTime( cur_hour,cur_min, cur_sec);


  if ((cur_hour != last_hour) || (cur_min != last_min)) {
    last_hour = cur_hour;
    last_min = cur_min;
    uhrzeit[0] = cur_hour / 10;
    uhrzeit[1] = cur_hour % 10;
    uhrzeit[2] = cur_min / 10;
    uhrzeit[3] = cur_min % 10;
    tft->fillScreen(ILI9486_BLACK);
    ball->SetY(20);
    blocks->Setup(uhrzeit);
    blocks->draw();
  }
  
}


void loop() {

  if (++loopcounter>30) {
  CheckTime();
  loopcounter=0;
  }

  curBlock = ball->move_draw();  
  blocks->checkBall(ball);
   
  paddle->update(ball->GetX());

  blocks->draw(ball);
  delay(5);

}


void tetristest() {
  tft->setRotation(1);
  tft->fillScreen(ILI9486_BLACK);
 
  /*tft->Tetris_setText("HELLO",false);
  tft->Tetris_drawText(60, 250, -1);
  
  while(!(tft->Tetris_drawText(60, 250, -1))) {
    delay(20);
    tft->Tetris_drawText(60, 250, ILI9486_BLACK);
  }
  */

  int16_t cur_hour;
  int16_t cur_min;
  int16_t cur_sec;
  GetTime( cur_hour,cur_min, cur_sec);

    uhrzeit[0] = cur_hour / 10;
    uhrzeit[1] = cur_hour % 10;
    uhrzeit[2] = cur_min / 10;
    uhrzeit[3] = cur_min % 10;
  //long nummer = (uhrzeit[0] *1000) + (uhrzeit[1]*100 ) + (uhrzeit[2]*10 ) + (uhrzeit[3] );
  //tft->Tetris_setNumbers(nummer);
  
  char timeString [8];
  sprintf(timeString, "%d%d:%d%d", uhrzeit[0], uhrzeit[1], uhrzeit[2], uhrzeit[3]);
  tft->Tetris_setTime(timeString);
  
  bool displaycolon = false;
  while(!(tft->Tetris_drawNumbers(60,250, displaycolon, -1))) {
    delay(20);
    GetTime( cur_hour,cur_min, cur_sec);
    displaycolon = ((cur_sec % 2) == 1);

    tft->Tetris_drawNumbers(60,250, displaycolon, ILI9486_BLACK);
  }
     delay(5000);
}