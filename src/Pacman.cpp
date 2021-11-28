#include <Arduino.h>

#include "Pacman.h"


bool mspacman = false;  //  if this is is set to true then play the game as Ms Pac-man
//Dot Array - There are 72 Dots with 4 of them that will turn Ghost Blue!
byte dot[73]; // Where if dot is zero then has been gobbled by Pac-Man

// Fruit flags
boolean fruitgone = false;
boolean fruitdrawn = false;
boolean fruiteatenpacman = false;

//Pacman & Ghost kill flags
boolean pacmanlost = false;
boolean ghostlost = false;

//Alarm setup variables
boolean xsetup = false; // Flag to determine if existing setup mode

// Scorecard
int pacmanscore = 0;
int ghostscore = 0;

int userspeedsetting = 1; // user can set normal, fast, crazy speed for the pacman animation

int gamespeed = 22; // Delay setting in mS for game default is 18
int cstep = 2; // Provides the resolution of the movement for the Ghost and Pacman character. Origially set to 2

// Animation delay to slow movement down
int dly = gamespeed; // Orignally 30


// Time Refresh counter 
int rfcvalue = 900; // wait this long untiul check time for changes
int rfc = 1;

// Pacman coordinates of top LHS of 28x28 bitmap
int xP = 4;
int yP = 108;
int P = 0;  // Pacman Graphic Flag 0 = Closed, 1 = Medium Open, 2 = Wide Open, 3 = Medium Open
int D = 0;  // Pacman direction 0 = right, 1 = down, 2 = left, 3 = up
int prevD;  // Capture legacy direction to enable adequate blanking of trail
int direct;   //  Random direction variable

// Graphics Drawing Variables

int Gposition = 0; // pointer to the undsguned short arraysholding bitmap of 784 pixels 28 x 28

// Ghost coordinates of top LHS of 28x28 bitmap
int xG = 288;
int yG = 108;
int GD = 2;  // Ghost direction 0 = right, 1 = down, 2 = left, 3 = up
int prevGD;  // Capture legacy direction to enable adequate blanking of trail
int gdirect;   //  Random direction variable 

// Declare global variables for previous time, to enable refesh of only digits that have changed
// There are four digits that bneed to be drawn independently to ensure consisitent positioning of time
  int c1 = 20;  // Tens hour digit
  int c2 = 20;  // Ones hour digit
  int c3 = 20;  // Tens minute digit
  int c4 = 20;  // Ones minute digit

uint16_t myicon[3] = { 0x00C0, 0xC000, 0x00F0};

Screen * myGLCD;

void pacman_init(Screen * mastertft) {

    myGLCD = mastertft;

    // Initialize Dot Array
  for (int dotarray = 1; dotarray < 73; dotarray++) {    
    dot[dotarray] = 1;    
    }
  drawscreen();

}

void pactest() {



    myGLCD->drawicon(5, 5, fruit);

 delay(2000);
}



// ===== initiateGame - Custom Function
void drawscreen() {


        myGLCD->drawRoundRect(0, 0, 319, 239, 2, TFT_BLUE); // X,Y location then X,Y Size 
        myGLCD->drawRoundRect(2, 2, 315, 235, 2, TFT_BLUE); 

// ---------------- Four top spacers and wall pillar

        myGLCD->drawRoundRect(35 , 35  , 25  , 10 , 2 ,  TFT_BLUE); 
        myGLCD->drawRoundRect(37 , 37  , 21  ,  6 , 2, TFT_BLUE);

        myGLCD->drawRoundRect(93 , 35  , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(95 , 37  , 21  , 6 , 2 , TFT_BLUE);

        myGLCD->drawRoundRect(201 , 35  , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(203 , 37  , 21  , 6 , 2 , TFT_BLUE);         

        myGLCD->drawRoundRect(258 , 35  , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(260 , 37  , 21  , 6 , 2 , TFT_BLUE); 

        myGLCD->drawRoundRect(155 , 0 , 10  , 45  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(157 , 2 , 6 , 41  , 2 , TFT_BLUE); 
 

// ---------------- Four bottom spacers and wall pillar

        myGLCD->drawRoundRect(35 , 196 , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(37 , 198 , 21  , 6 , 2 , TFT_BLUE);

        myGLCD->drawRoundRect(93 , 196 , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(95 , 198 , 21  , 6 , 2 , TFT_BLUE);

        myGLCD->drawRoundRect(201 , 196 , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(203 , 198 , 21  , 6 , 2 , TFT_BLUE);          

        myGLCD->drawRoundRect(258 , 196 , 25  , 10  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(260 , 198 , 21  , 6 , 2 ,TFT_BLUE);          

        myGLCD->drawRoundRect(155 , 196 , 10  , 43  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(157 , 198 , 6 , 39  , 2 , TFT_BLUE); 


// ---------- Four Door Pillars 

        myGLCD->drawRoundRect(0 , 80  , 27  , 25  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(2 , 85  , 23  , 15  , 2 , TFT_BLUE); 

        myGLCD->drawRoundRect(0 , 140 , 27  , 25  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(2 , 145 , 23  , 15  , 2 , TFT_BLUE); 
        
        myGLCD->drawRoundRect(292 , 80  , 27  , 25  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(294 , 85  , 23  , 15  , 2 , TFT_BLUE); 
        
        myGLCD->drawRoundRect(292 , 140 , 27  , 25  , 2 , TFT_BLUE); 
        myGLCD->drawRoundRect(294 , 145 , 23  , 15  , 2 , TFT_BLUE);  

// ---------------- Clear lines on Outside wall  
        myGLCD->drawRoundRect(1 , 1 , 317 , 237 , 2 , TFT_BLACK);  
        
        myGLCD->fillRect(0  , 106 , 3 , 33  ,  TFT_BLACK); 
        myGLCD->fillRect(316  , 106 , 3 , 33 , TFT_BLACK); 

// Row 1
if ( dot[1] == 1) {
  myGLCD->fillCircle(19, 19, 2,TFT_GREY); // dot 1
  }
if ( dot[2] == 1) {  
  myGLCD->fillCircle(42, 19, 2,TFT_GREY); // dot 2
  }
if ( dot[3] == 1) {
  myGLCD->fillCircle(65, 19, 2,TFT_GREY); // dot 3
  }
if ( dot[4] == 1) {
  myGLCD->fillCircle(88, 19, 2,TFT_GREY); // dot 4
  }
if ( dot[5] == 1) {
  myGLCD->fillCircle(112, 19, 2,TFT_GREY); // dot 5
  }
if ( dot[6] == 1) {
  myGLCD->fillCircle(136, 19, 2,TFT_GREY); // dot 6   
  }  
// 
if ( dot[7] == 1) {
  myGLCD->fillCircle(183, 19, 2,TFT_GREY); // dot 7
  }
if ( dot[8] == 1) {  
  myGLCD->fillCircle(206, 19, 2,TFT_GREY);  // dot 8 
  }
if ( dot[9] == 1) {  
  myGLCD->fillCircle(229, 19, 2,TFT_GREY); // dot 9
  }
if ( dot[10] == 1) {  
  myGLCD->fillCircle(252, 19, 2,TFT_GREY); // dot 10
  }
if ( dot[11] == 1) {  
  myGLCD->fillCircle(275, 19, 2,TFT_GREY);  // dot 11
  }
if ( dot[12] == 1) {
  myGLCD->fillCircle(298, 19, 2,TFT_GREY);  // dot 12
  }
// Row 2
if ( dot[13] == 1) {
  myGLCD->fillCircle(19, 40, 7,TFT_GREY); // Big dot 13
  }
if ( dot[14] == 1) {
  myGLCD->fillCircle(77, 40, 2,TFT_GREY);  // dot 14
  }
if ( dot[15] == 1) {
  myGLCD->fillCircle(136, 40, 2,TFT_GREY);  // dot 15
  }
if ( dot[16] == 1) {
  myGLCD->fillCircle(183, 40, 2,TFT_GREY);  // dot 16
  }
if ( dot[17] == 1) {
  myGLCD->fillCircle(241, 40, 2,TFT_GREY);  // dot 17
  }
if ( dot[18] == 1) {
  myGLCD->fillCircle(298, 40, 7,TFT_GREY); // Big dot 18
  }  

  
// Row 3

if ( dot[19] == 1) {
  myGLCD->fillCircle(19, 60, 2,TFT_GREY);
}
if ( dot[20] == 1) {
  myGLCD->fillCircle(42, 60, 2,TFT_GREY);
}
if ( dot[21] == 1) {
  myGLCD->fillCircle(65, 60, 2,TFT_GREY); 
}
if ( dot[22] == 1) {
  myGLCD->fillCircle(88, 60, 2,TFT_GREY);
}
if ( dot[23] == 1) {
  myGLCD->fillCircle(112, 60, 2,TFT_GREY);
}
if ( dot[24] == 1) {
  myGLCD->fillCircle(136, 60, 2,TFT_GREY); 
}
if ( dot[25] == 1) { 
  myGLCD->fillCircle(160, 60, 2,TFT_GREY);
}
if ( dot[26] == 1) {
  myGLCD->fillCircle(183, 60, 2,TFT_GREY);
}
if ( dot[27] == 1) {
  myGLCD->fillCircle(206, 60, 2,TFT_GREY);  
}
if ( dot[28] == 1) {
  myGLCD->fillCircle(229, 60, 2,TFT_GREY);
}
if ( dot[29] == 1) {
  myGLCD->fillCircle(252, 60, 2,TFT_GREY);
}
if ( dot[30] == 1) {
  myGLCD->fillCircle(275, 60, 2,TFT_GREY); 
}
if ( dot[31] == 1) {
  myGLCD->fillCircle(298, 60, 2,TFT_GREY);   
}

// Row 4
if ( dot[32] == 1) {
  myGLCD->fillCircle(42, 80, 2,TFT_GREY);
}
if ( dot[33] == 1) {
  myGLCD->fillCircle(275, 80, 2,TFT_GREY);   
}
// Row 5
if ( dot[34] == 1) {
  myGLCD->fillCircle(42, 100, 2,TFT_GREY);
}
if ( dot[35] == 1) {
  myGLCD->fillCircle(275, 100, 2,TFT_GREY);
}
// Row 6
if ( dot[36] == 1) {
  myGLCD->fillCircle(42, 120, 2,TFT_GREY);
}
if ( dot[37] == 1) {
  myGLCD->fillCircle(275, 120, 2,TFT_GREY);
}
// Row 7
if ( dot[38] == 1) {
  myGLCD->fillCircle(42, 140, 2,TFT_GREY);
}
if ( dot[39] == 1) {
  myGLCD->fillCircle(275, 140, 2,TFT_GREY);
}
// Row 8
if ( dot[40] == 1) {
  myGLCD->fillCircle(42, 160, 2,TFT_GREY);
}
if ( dot[41] == 1) {
  myGLCD->fillCircle(275, 160, 2,TFT_GREY);
}
// Row 9
if ( dot[42] == 1) {
  myGLCD->fillCircle(19, 181, 2,TFT_GREY);
}
if ( dot[43] == 1) {
  myGLCD->fillCircle(42, 181, 2,TFT_GREY);
}
if ( dot[44] == 1) {
  myGLCD->fillCircle(65, 181, 2,TFT_GREY); 
}
if ( dot[45] == 1) {
  myGLCD->fillCircle(88, 181, 2,TFT_GREY);
}
if ( dot[46] == 1) {
  myGLCD->fillCircle(112, 181, 2,TFT_GREY);
}
if ( dot[47] == 1) {
  myGLCD->fillCircle(136, 181, 2,TFT_GREY); 
}
if ( dot[48] == 1) { 
  myGLCD->fillCircle(160, 181, 2,TFT_GREY);
}
if ( dot[49] == 1) {
  myGLCD->fillCircle(183, 181, 2,TFT_GREY);
}
if ( dot[50] == 1) {
  myGLCD->fillCircle(206, 181, 2,TFT_GREY);  
}
if ( dot[51] == 1) {
  myGLCD->fillCircle(229, 181, 2,TFT_GREY);
}
if ( dot[52] == 1) {
  myGLCD->fillCircle(252, 181, 2,TFT_GREY);
}
if ( dot[53] == 1) {
  myGLCD->fillCircle(275, 181, 2,TFT_GREY); 
}
if ( dot[54] == 1) {
  myGLCD->fillCircle(298, 181, 2,TFT_GREY);   
}
// Row 10
if ( dot[55] == 1) {
  myGLCD->fillCircle(19, 201, 7,TFT_GREY); // Big dot
}
if ( dot[56] == 1) {
  myGLCD->fillCircle(77, 201, 2,TFT_GREY);
}
if ( dot[57] == 1) {
  myGLCD->fillCircle(136, 201, 2,TFT_GREY);
}
if ( dot[58] == 1) {
  myGLCD->fillCircle(183, 201, 2,TFT_GREY);
}
if ( dot[59] == 1) {
  myGLCD->fillCircle(241, 201, 2,TFT_GREY);
}
if ( dot[60] == 1) {
  myGLCD->fillCircle(298, 201, 7,TFT_GREY); // Big dot
}  

  

 
  // Row 11
if ( dot[61] == 1) {
  myGLCD->fillCircle(19, 221, 2,TFT_GREY);
}
if ( dot[62] == 1) {
  myGLCD->fillCircle(42, 221, 2,TFT_GREY);
}
if ( dot[63] == 1) {
  myGLCD->fillCircle(65, 221, 2,TFT_GREY); 
}
if ( dot[64] == 1) { 
  myGLCD->fillCircle(88, 221, 2,TFT_GREY);
}
if ( dot[65] == 1) {
  myGLCD->fillCircle(112, 221, 2,TFT_GREY);
}
if ( dot[66] == 1) {
  myGLCD->fillCircle(136, 221, 2,TFT_GREY);   
}  
//  myGLCD->fillCircle(160, 19, 2,TFT_GREY);

if ( dot[67] == 1) {
  myGLCD->fillCircle(183, 221, 2,TFT_GREY);
}
if ( dot[68] == 1) {
  myGLCD->fillCircle(206, 221, 2,TFT_GREY);  
}
if ( dot[69] == 1) {
  myGLCD->fillCircle(229, 221, 2,TFT_GREY);
}
if ( dot[70] == 1) {
  myGLCD->fillCircle(252, 221, 2,TFT_GREY);
}
if ( dot[71] == 1) {
  myGLCD->fillCircle(275, 221, 2,TFT_GREY); 
}
if ( dot[72] == 1) {
  myGLCD->fillCircle(298, 221, 2,TFT_GREY); 
}

 }
 