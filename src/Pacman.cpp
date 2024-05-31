// based on https://www.instructables.com/ESP32-Pacman-Clock/

#include <Arduino.h>

#include "Pacman.h"

extern byte uhrzeit[6];


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
Serial.println("packman init");

    myGLCD = mastertft;
    myGLCD->fillScreen(ILI9486_BLACK);
    myGLCD->setOffset(0, 0);  // 60;40

   #ifdef rotate
   myGLCD->setRotation(3);   // reverse !!!
   #else
   myGLCD->setRotation(1); 
   #endif

    // Initialize Dot Array
  for (int dotarray = 1; dotarray < 73; dotarray++) {    
    dot[dotarray] = 1;    
    }

  c1=-1; c2=-1; c3=-1; c4=-1;

  setgamespeed(random(2)+1);  // new game between 0-5
  
  drawscreen();
  UpdateDisp();
}

void setgamespeed(short userspeedsetting){

 if (userspeedsetting == 1) {
  gamespeed = 22;
} else
if (userspeedsetting == 2) {
  gamespeed = 14;
} else
if (userspeedsetting == 3) {
  gamespeed = 0;
}

dly = gamespeed; // Reset the game speed  
}

void pacman_run() {
    UpdateDisp();
    printscoreboard();
    drawfruit(); 
    refreshgame();
    displaypacman();
    displayghost();
    delay(dly);
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
 


void UpdateDisp(){
  int pm = 0; // Flag to detrmine if PM or AM
  
  // There are four digits that need to be drawn independently to ensure consisitent positioning of time
  int d1 = uhrzeit[0];  // Tens hour digit
  int d2 = uhrzeit[1];  // Ones hour digit
  int d3 = uhrzeit[2];  // Tens minute digit
  int d4 = uhrzeit[3];  // Ones minute digit


// *************************************************************************
// Print each digit if it has changed to reduce screen impact/flicker

 myGLCD->setTextColor(TFT_WHITE,TFT_BLACK); 
 myGLCD->setTextSize(2);
 myGLCD->setFreeFontFix();  // FF20

// First Digit
if(((d1 != c1)||(xsetup == true))&&(d1 != 0)){ // Do not print zero in first digit position
    myGLCD->drawNumber(d1,51,85); // Printing thisnumber impacts LFH walls so redraw impacted area   
// ---------------- reprint two left wall pillars
    
    myGLCD->drawRoundRect(0 , 80  , 27  , 25  , 2 , TFT_BLUE); 
    myGLCD->drawRoundRect(2 , 85  , 23  , 15  , 2 , TFT_BLUE); 

    myGLCD->drawRoundRect(0 , 140 , 27  , 25  , 2 , TFT_BLUE); 
    myGLCD->drawRoundRect(2 , 145 , 23  , 15  , 2 , TFT_BLUE); 

// ---------------- Clear lines on Outside wall
    myGLCD->drawRoundRect(1 , 1 , 317 , 237 , 2 , TFT_BLACK); 



}
//If prevous time 23:59 or 00:59 and change in time then blank First Digit

if((c1 == 2) && (c2 == 3) && (c3 == 5) && (c4 == 9) && (d2 != c2) ){ // Clear the previouis First Digit and redraw wall
    myGLCD->fillRect(50  , 70  , 45  , 95, TFT_BLACK);
}

if((c1 == 0) && (c2 == 0) && (c3 == 5) && (c4 == 9) && (d2 != c2) ){ // Clear the previouis First Digit and redraw wall
    myGLCD->fillRect(50  , 70  , 45  , 95, TFT_BLACK);
}

// Reprint the dots that have not been gobbled
// Row 4
if ( dot[32] == 1) {
  myGLCD->fillCircle(42, 80, 2,TFT_GREY);
} 

// Row 5

if ( dot[34] == 1) {
  myGLCD->fillCircle(42, 100, 2,TFT_GREY);
}

// Row 6
if ( dot[36] == 1) {
  myGLCD->fillCircle(42, 120, 2,TFT_GREY);
}

// Row 7
if ( dot[38] == 1) {
  myGLCD->fillCircle(42, 140, 2,TFT_GREY);
}

// Row 8
if ( dot[40] == 1) {
  myGLCD->fillCircle(42, 160, 2,TFT_GREY);
}


 myGLCD->setTextColor(TFT_WHITE,TFT_BLACK);  // myGLCD->setTextSize(20);
  
// Second Digit
if((d2 != c2)||(xsetup == true)){
  myGLCD->drawNumber(d2,91,85); // Print 0
}

// Third Digit
if((d3 != c3)||(xsetup == true)){
  myGLCD->drawNumber(d3,156,85); // Was 145    
}

// Fourth Digit
if((d4 != c4)||(xsetup == true)){
  myGLCD->drawNumber(d4,211,85); // Was 205  
}

if (xsetup == true){
  xsetup = false; // Reset Flag now leaving setup mode
  } 


  // Round dots

  myGLCD->fillCircle(148, 112, 4,TFT_WHITE);
  myGLCD->fillCircle(148, 132, 4,TFT_WHITE);

//--------------------- copy exising time digits to global variables so that these can be used to test which digits change in future

c1 = d1;
c2 = d2;
c3 = d3;
c4 = d4;

 myGLCD->setTextColor(TFT_WHITE,TFT_BLACK); 
 myGLCD->setTextSize(1);
 myGLCD->clearFreeFont();

}



void printscoreboard(){ //Print scoreboard

if((ghostscore >= 95)||(pacmanscore >= 95)){ // Reset scoreboard if over 95
        ghostscore = 0;
        pacmanscore = 0;
  
    for (int dotarray = 1; dotarray < 73; dotarray++) {
      dot[dotarray] = 1;      
      }
  
  // Blank the screen across the digits before redrawing them
      myGLCD->fillRect(299  , 87  , 15  , 10  , TFT_BLACK); // Blankout ghost score  
      myGLCD->fillRect(7  , 87  , 15  , 10  , TFT_BLACK);   // Blankout pacman score
  
     drawscreen(); // Redraw dots  
  }
  

   myGLCD->setTextColor(TFT_RED,TFT_BLACK);   myGLCD->setTextSize(1);
  
  // Account for position issue if over or under 10
  
  if (ghostscore >= 10){
    myGLCD->drawNumber(ghostscore,301,88); 
  } else {
    myGLCD->drawNumber(ghostscore,307,88);  // Account for being less than 10
  }
  
   myGLCD->setTextColor(TFT_YELLOW,TFT_BLACK);   myGLCD->setTextSize(1);
  
  if (pacmanscore >= 10){
    myGLCD->drawNumber(pacmanscore,9,88);  
  } else{
    myGLCD->drawNumber(pacmanscore,15,88);  // Account for being less than 10
  } 
}


void drawfruit(){  // Draw fruit and allocate points

     // Draw fruit
  if ((fruitdrawn == false)&&(fruitgone == false)){ // draw fruit and set flag that fruit present so its not drawn again
      myGLCD->drawicon(146, 168, fruit); //   draw fruit 
      fruitdrawn = true;
  }  
  
  // Redraw fruit if Ghost eats fruit only if Ghost passesover 172 or 120 on the row 186
  if ((fruitdrawn == true)&&(fruitgone == false)&&(xG >= 168)&&(xG <= 170)&&(yG >= 168)&&(yG <= 180)){
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
  }
  
  if ((fruitdrawn == true)&&(fruitgone == false)&&(xG == 120)&&(yG == 168)){
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
  }
  
  // Award Points if Pacman eats Big Dots
  if ((fruitdrawn == true)&&(fruitgone == false)&&(xP == 146)&&(yP == 168)){
    fruitgone = true; // If Pacman eats fruit then fruit disappears  
    pacmanscore = pacmanscore + 5; //Increment pacman score 
  }
 
}

void refreshgame(){ // Read the current date and time from the RTC and reset board

// Read the current date and time from the RTC and reset board
rfc++;
  if (rfc >= rfcvalue) { // count cycles and print time
    UpdateDisp(); // update value to clock then ...
     fruiteatenpacman =  false; // Turn Ghost red again  
     fruitdrawn = false; // If Pacman eats fruit then fruit disappears
     fruitgone = false;
     // Reset every minute both characters
     pacmanlost = false;
     ghostlost = false;
     dly = gamespeed; // reset delay
     rfc = 0;
  }
}


void displaypacman(){ // Draw Pacman in position on screen
 // Pacman Captured
// If pacman captured then pacman dissapears until reset
if ((fruiteatenpacman == false)&&(abs(xG-xP)<=5)&&(abs(yG-yP)<=5)){ 
// firstly blank out Pacman
//    myGLCD->setColor(0,0,0);
    myGLCD->fillRect(xP, yP, 28, 28, TFT_BLACK); 

  if (pacmanlost == false){
    ghostscore = ghostscore + 15;  
  }
  pacmanlost = true;
 // Slow down speed of drawing now only one moving charater
  dly = gamespeed;
  }
 
if (pacmanlost == false) { // Only draw pacman if he is still alive


// Draw Pac-Man
drawPacman(xP,yP,P,D,prevD); // Draws Pacman at these coordinates
  

// If Pac-Man is on a dot then print the adjacent dots if they are valid

//  myGLCD->setColor(200, 200, 200);
  
// Check Rows

if (yP== 4) {  // if in Row 1 **********************************************************
  if (xP== 4) { // dot 1
     if (dot[2] == 1) {  // Check if dot 2 gobbled already
    myGLCD->fillCircle(42, 19, 2, TFT_GREY); // dot 2
     }    
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     }    

  } else
  if (xP== 28) { // dot 2
     if (dot[1] == 1) {  // Check if dot 1 gobbled already
    myGLCD->fillCircle(19, 19, 2, TFT_GREY); // dot 1
     }    
      if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }    

  } else
  if (xP== 52) { // dot 3
     if (dot[2] == 1) {  // Check if dot 2 gobbled already
    myGLCD->fillCircle(42, 19, 2, TFT_GREY); // dot 2
     }    
      if (dot[4] == 1) {  // Check if dot 4 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }   
  } else
  if (xP== 74) { // dot 4
     if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }    
      if (dot[5] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(112, 19, 2, TFT_GREY); // dot 5
     }   
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }    
  } else
  if (xP== 98) { // dot 5
     if (dot[4] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     }    
      if (dot[6] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(136, 19, 2, TFT_GREY); // dot 6
     }     
  } else
  if (xP== 120) { // dot 6
     if (dot[5] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(112, 19, 2, TFT_GREY); // dot 5
     }    
      if (dot[15] == 1) {  // Check if dot 15 gobbled already
    myGLCD->fillCircle(136, 40, 2, TFT_GREY); // dot 15
     }     
  } else
 

 if (xP== 168) { // dot 7
      if (dot[16] == 1) {  // Check if dot 16 gobbled already
    myGLCD->fillCircle(183, 40, 2, TFT_GREY); // dot 16
     }    
      if (dot[8] == 1) {  // Check if dot 8 gobbled already
    myGLCD->fillCircle(206, 19, 2, TFT_GREY); // dot 8
     }     
  } else
  if (xP== 192) { // dot 8
      if (dot[7] == 1) {  // Check if dot 7 gobbled already
    myGLCD->fillCircle(183, 19, 2, TFT_GREY); // dot 7
     }    
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }    
  } else
  if (xP== 216) { // dot 9
      if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }    
      if (dot[8] == 1) {  // Check if dot 8 gobbled already
    myGLCD->fillCircle(206, 19, 2, TFT_GREY); // dot 8
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
 } else
  if (xP== 238) { // dot 10
      if (dot[11] == 1) {  // Check if dot 11 gobbled already
    myGLCD->fillCircle(275, 19, 2, TFT_GREY); // dot 11
     }    
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
  } else
  if (xP== 262) { // dot 11
      if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }    
      if (dot[12] == 1) {  // Check if dot 12 gobbled already
    myGLCD->fillCircle(298, 19, 2, TFT_GREY); // dot 12
     }    
      if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
     } 
  } else
  if (xP== 284) { // dot 12
      if (dot[11] == 1) {  // Check if dot 11 gobbled already
    myGLCD->fillCircle(275, 19, 2, TFT_GREY); // dot 11
     }    
      if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
     }  
  }
} else 
if (yP== 26) {  // if in Row 2  **********************************************************
  if (xP== 4) { // dot 13
     if (dot[1] == 1) {  // Check if dot 1 gobbled already
    myGLCD->fillCircle(19, 19, 2, TFT_GREY); // dot 1
     }    
      if (dot[19] == 1) {  // Check if dot 19 gobbled already
    myGLCD->fillCircle(19, 60, 2, TFT_GREY); //  dot 19
     }   
  } else
  
    if (xP== 62) { // dot 14
      if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }   
         if (dot[4] == 1) {  // Check if dot 4 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     } 
         if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }   
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     }    
     
  } else
  
  if (xP== 120) { // dot 15
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[6] == 1) {  // Check if dot 6 gobbled already
    myGLCD->fillCircle(136, 19, 2, TFT_GREY); // dot 6
     }      
  } else
  if (xP== 168) { // dot 16
      if (dot[7] == 1) {  // Check if dot 7 gobbled already
    myGLCD->fillCircle(183, 19, 2, TFT_GREY); // dot 7
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }          
  } else
    if (xP== 228) { // dot 17
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }      
       if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }  
      if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }  
       if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }     
     
  } else
  if (xP== 284) { // dot 18
      if (dot[31] == 1) {  // Check if dot 31 gobbled already
    myGLCD->fillCircle(298, 60, 2, TFT_GREY); // dot 31
     }    
      if (dot[12] == 1) {  // Check if dot 12 gobbled already
    myGLCD->fillCircle(298, 19, 2, TFT_GREY); // dot 12
     }  
  }
} else
if (yP== 46) {  // if in Row 3  **********************************************************
  if (xP== 4) { // dot 19
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }    
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     }  
  } else
  if (xP== 28) { // dot 20
     if (dot[19] == 1) {  // Check if dot 19 gobbled already
    myGLCD->fillCircle(19, 60, 2, TFT_GREY); // dot 19
     }    
      if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }   
      if (dot[32] == 1) {  // Check if dot 32 gobbled already
    myGLCD->fillCircle(42, 80, 2, TFT_GREY); // dot 32
     }    
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     } 
  } else
  if (xP== 52) { // dot 21
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }    
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }         
  } else
  if (xP== 74) { // dot 22
      if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }    
      if (dot[23] == 1) {  // Check if dot 23 gobbled already
    myGLCD->fillCircle(112, 60, 2, TFT_GREY); // dot 23
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }    
  } else
  if (xP== 98) { // dot 23
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     }  
    
  } else
  if (xP== 120) { // dot 24
      if (dot[25] == 1) {  // Check if dot 25 gobbled already
    myGLCD->fillCircle(160, 60, 2, TFT_GREY); // dot 25
     }    
      if (dot[23] == 1) {  // Check if dot 23 gobbled already
    myGLCD->fillCircle(112, 60, 2, TFT_GREY); // dot 23
     }
      if (dot[15] == 1) {  // Check if dot 15 gobbled already
    myGLCD->fillCircle(136, 40, 2, TFT_GREY); // dot 15
     }        
  } else
  if (xP== 146) { // dot 25
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }    
  } else
  if (xP== 168) { // dot 26
      if (dot[25] == 1) {  // Check if dot 25 gobbled already
    myGLCD->fillCircle(160, 60, 2, TFT_GREY); // dot 25
     }    
      if (dot[27] == 1) {  // Check if dot 27 gobbled already
    myGLCD->fillCircle(206, 60, 2, TFT_GREY); // dot 27
     }
      if (dot[16] == 1) {  // Check if dot 16 gobbled already
    myGLCD->fillCircle(183, 40, 2, TFT_GREY); // dot 16
     }    
  } else
  if (xP== 192) { // dot 27
     if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }      
  } else
  if (xP== 216) { // dot 28
      if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }    
      if (dot[27] == 1) {  // Check if dot 27 gobbled already
    myGLCD->fillCircle(206, 60, 2, TFT_GREY); // dot 27
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
  } else
  if (xP== 238) { // dot 29
     if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }    
      if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
   
  } else
  if (xP== 262) { // dot 30
      if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }    
      if (dot[33] == 1) {  // Check if dot 33 gobbled already
    myGLCD->fillCircle(275, 80, 2, TFT_GREY); // dot 33
     }      
      if (dot[31] == 1) {  // Check if dot 31 gobbled already
    myGLCD->fillCircle(298, 60, 2, TFT_GREY); // dot 31
     }  
  
  } else
  if (xP== 284) { // dot 31
   if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
   }     
   if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
   } 
  }
} else

if (yP== 168) {  // if in Row 4  **********************************************************
  if (xP== 4) { // dot 42
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }     
     if (dot[55] == 1) {  // Check if dot 55 gobbled already
    myGLCD->fillCircle(19, 201, 7, TFT_GREY); // dot 55
     }     
  } else
  if (xP== 28) { // dot 43
     if (dot[42] == 1) {  // Check if dot 42 gobbled already
    myGLCD->fillCircle(19, 181, 2, TFT_GREY); // dot 42
     }     
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     }   
      if (dot[40] == 1) {  // Check if dot 40 gobbled already
    myGLCD->fillCircle(42, 160, 2, TFT_GREY); // dot 40
     }       
  } else
  if (xP== 52) { // dot 44
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }     
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     } 
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }    
  } else
  if (xP== 74) { // dot 45
     if (dot[46] == 1) {  // Check if dot 46 gobbled already
    myGLCD->fillCircle(112, 181, 2, TFT_GREY); // dot 46
     }     
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     } 
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }    
     
  } else
  if (xP== 98) { // dot 46
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     }  
  } else
  if (xP== 120) { // dot 47
     if (dot[48] == 1) {  // Check if dot 48 gobbled already
    myGLCD->fillCircle(160, 181, 2, TFT_GREY); // dot 48
     }     
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
     if (dot[46] == 1) {  // Check if dot 46 gobbled already
    myGLCD->fillCircle(112, 181, 2, TFT_GREY); // dot 46
     } 
     if (dot[57] == 1) {  // Check if dot 57 gobbled already
    myGLCD->fillCircle(136, 201, 2, TFT_GREY); // dot 57 
     }      
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  if (xP== 146) { // dot 48
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }  
  } else

  if (xP== 168) { // dot 49
     if (dot[48] == 1) {  // Check if dot 48 gobbled already
    myGLCD->fillCircle(160, 181, 2, TFT_GREY); // dot 48
     }     
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
     if (dot[50] == 1) {  // Check if dot 50 gobbled already
    myGLCD->fillCircle(206, 181, 2, TFT_GREY); // dot 50
     } 
     if (dot[58] == 1) {  // Check if dot 58 gobbled already
    myGLCD->fillCircle(183, 201, 2, TFT_GREY); // dot 58
     }        
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  if (xP== 192) { // dot 50
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }      
  } else
  if (xP== 216) { // dot 51
     if (dot[50] == 1) {  // Check if dot 50 gobbled already
    myGLCD->fillCircle(206, 181, 2, TFT_GREY); // dot 50
     }    
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }     
  } else
  if (xP== 238) { // dot 52
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }    
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }  
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }     
  } else
 
 if (xP== 262) { // dot 53
     if (dot[41] == 1) {  // Check if dot 41 gobbled already
    myGLCD->fillCircle(275, 160, 2, TFT_GREY); // dot 41
     }    
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[54] == 1) {  // Check if dot 54 gobbled already
    myGLCD->fillCircle(298, 181, 2, TFT_GREY); // dot 54
     }  
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }        
  } else
  if (xP== 284) { // dot 54
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }    
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }      
  } 

} else
if (yP== 188) {  // if in Row 5  **********************************************************
  if (xP== 4) { // dot 55
     if (dot[42] == 1) {  // Check if dot 42 gobbled already
    myGLCD->fillCircle(19, 181, 2, TFT_GREY); // dot 42
     } 
     if (dot[61] == 1) {  // Check if dot 61 gobbled already
    myGLCD->fillCircle(19, 221, 2, TFT_GREY); // dot 61
     }    
  } else
   if (xP== 62) { // dot 56
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     } 
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     } 
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     }      
     
  } else
  
  if (xP== 120) { // dot 57
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[66] == 1) {  // Check if dot 66 gobbled already
    myGLCD->fillCircle(136, 221, 2, TFT_GREY); // dot 66
     }    
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  if (xP== 168) { // dot 58
     if (dot[67] == 1) {  // Check if dot 67 gobbled already
    myGLCD->fillCircle(183, 221, 2, TFT_GREY); // dot 67
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }       
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
        myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  
  if (xP== 228) { // dot 59
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     } 
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }      
     
  } else
  
  if (xP== 284) { // dot 60
     if (dot[72] == 1) {  // Check if dot 72 gobbled already
    myGLCD->fillCircle(298, 221, 7, TFT_GREY); // Big dot 72
     } 
     if (dot[54] == 1) {  // Check if dot 54 gobbled already
    myGLCD->fillCircle(298, 181, 2, TFT_GREY); // dot 54
     }    
  } 

} else


if (yP== 208) {  // if in Row 6  **********************************************************
  if (xP== 4) { // dot 61
     if (dot[55] == 1) {  // Check if dot 55 gobbled already
    myGLCD->fillCircle(19, 201, 7, TFT_GREY); // dot 55
     } 
     if (dot[62] == 1) {  // Check if dot 62 gobbled already
    myGLCD->fillCircle(42, 221, 2, TFT_GREY); // dot 62
     }   
  } else
  if (xP== 28) { // dot 62
     if (dot[61] == 1) {  // Check if dot 61 gobbled already
    myGLCD->fillCircle(19, 221, 2, TFT_GREY); // dot 61
     }  
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }      
  } else
  if (xP== 52) { // dot 63
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     } 
     if (dot[62] == 1) {  // Check if dot 62 gobbled already
    myGLCD->fillCircle(42, 221, 2, TFT_GREY); // dot 62
     }  
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }      
  } else
  if (xP== 74) { // dot 64
     if (dot[65] == 1) {  // Check if dot 65 gobbled already
    myGLCD->fillCircle(112, 221, 2, TFT_GREY); // dot 65
     } 
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }  
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }     
  } else
  if (xP== 98) { // dot 65
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     } 
     if (dot[66] == 1) {  // Check if dot 66 gobbled already
    myGLCD->fillCircle(136, 221, 2, TFT_GREY); // dot 66
     }    
  } else
  if (xP== 120) { // dot 66
     if (dot[65] == 1) {  // Check if dot 65 gobbled already
    myGLCD->fillCircle(112, 221, 2, TFT_GREY); // dot 65
     } 
     if (dot[57] == 1) {  // Check if dot 57 gobbled already
    myGLCD->fillCircle(136, 201, 2, TFT_GREY); // dot 57 
     }    
  } else
  if (xP== 168) { // dot 67
     if (dot[68] == 1) {  // Check if dot 68 gobbled already
    myGLCD->fillCircle(206, 221, 2, TFT_GREY); // dot 68
     } 
     if (dot[58] == 1) {  // Check if dot 58 gobbled already
    myGLCD->fillCircle(183, 201, 2, TFT_GREY); // dot 58
     }     
  } else
  if (xP== 192) { // dot 68
     if (dot[67] == 1) {  // Check if dot 67 gobbled already
    myGLCD->fillCircle(183, 221, 2, TFT_GREY); // dot 67
     } 
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     }    
  } else
  if (xP== 216) { // dot 69
     if (dot[68] == 1) {  // Check if dot 68 gobbled already
    myGLCD->fillCircle(206, 221, 2, TFT_GREY); // dot 68
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }    
  } else
  if (xP== 238) { // dot 70
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }
     if (dot[71] == 1) {  // Check if dot 71 gobbled already
    myGLCD->fillCircle(275, 221, 2, TFT_GREY); // dot 71
     }       
  } else
  if (xP== 262) { // dot 71
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }  
     if (dot[72] == 1) {  // Check if dot 72 gobbled already
    myGLCD->fillCircle(298, 221, 2, TFT_GREY); // dot 72
     }       
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }
  } else
  if (xP== 284) { // dot 72
     if (dot[71] == 1) {  // Check if dot 71 gobbled already
    myGLCD->fillCircle(275, 221, 2, TFT_GREY); // dot 71
     } 
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }     
  }
} else



// Check Columns


if (xP== 28) {  // if in Column 2
  if (yP== 66) { // dot 32
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }     
     if (dot[34] == 1) {  // Check if dot 34 gobbled already
    myGLCD->fillCircle(42, 100, 2, TFT_GREY); // dot 34
     }        
  } else
  if (yP== 86) { // dot 34
      if (dot[32] == 1) {  // Check if dot 32 gobbled already
    myGLCD->fillCircle(42, 80, 2, TFT_GREY); // dot 32
     }  
      if (dot[36] == 1) {  // Check if dot 36 gobbled already
    myGLCD->fillCircle(42, 120, 2, TFT_GREY); // dot 36
     }      
  } else
  if (yP== 106) { // dot 36
     if (dot[38] == 1) {  // Check if dot 38 gobbled already
    myGLCD->fillCircle(42, 140, 2, TFT_GREY); // dot 38
     }     
     if (dot[34] == 1) {  // Check if dot 34 gobbled already
    myGLCD->fillCircle(42, 100, 2, TFT_GREY); // dot 34
     }      
  } else
  if (yP== 126) { // dot 38
      if (dot[40] == 1) {  // Check if dot 40 gobbled already
    myGLCD->fillCircle(42, 160, 2, TFT_GREY); // dot 40
     } 
      if (dot[36] == 1) {  // Check if dot 36 gobbled already
    myGLCD->fillCircle(42, 120, 2, TFT_GREY); // dot 36
     }       
  } else
  if (yP== 146) { // dot 40
     if (dot[38] == 1) {  // Check if dot 38 gobbled already
    myGLCD->fillCircle(42, 140, 2, TFT_GREY); // dot 38
     }     
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }      
  } 

} else
if (xP== 262) {  // if in Column 7

  if (yP== 66) { // dot 33
      if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
     }   
      if (dot[35] == 1) {  // Check if dot 35 gobbled already
    myGLCD->fillCircle(275, 100, 2, TFT_GREY); // dot 35
     }   
  } else
  if (yP== 86) { // dot 35
      if (dot[33] == 1) {  // Check if dot 33 gobbled already
    myGLCD->fillCircle(275, 80, 2, TFT_GREY); // dot 33
     }  
      if (dot[37] == 1) {  // Check if dot 37 gobbled already
    myGLCD->fillCircle(275, 120, 2, TFT_GREY); // dot 37
     }     
  } else
  if (yP== 106) { // dot 37
      if (dot[35] == 1) {  // Check if dot 35 gobbled already
    myGLCD->fillCircle(275, 100, 2, TFT_GREY); // dot 35
     }  
      if (dot[39] == 1) {  // Check if dot 39 gobbled already
    myGLCD->fillCircle(275, 140, 2, TFT_GREY); // dot 39
     }      
  } else
  if (yP== 126) { // dot 39
      if (dot[37] == 1) {  // Check if dot 37 gobbled already
    myGLCD->fillCircle(275, 120, 2, TFT_GREY); // dot 37
     }
     if (dot[41] == 1) {  // Check if dot 41 gobbled already
    myGLCD->fillCircle(275, 160, 2, TFT_GREY); // dot 41
     }       
  } else
  if (yP== 146) { // dot 41
      if (dot[39] == 1) {  // Check if dot 39 gobbled already
    myGLCD->fillCircle(275, 140, 2, TFT_GREY); // dot 39
     } 
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }     
  } 
}



  
// increment Pacman Graphic Flag 0 = Closed, 1 = Medium Open, 2 = Wide Open
P=P+1; 
if(P==4){
  P=0; // Reset counter to closed
}

      
       
// Capture legacy direction to enable adequate blanking of trail
prevD = D;

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);

  myGLCD->drawString(xT,100,140); // Print xP
  myGLCD->drawString(yT,155,140); // Print yP 
*/ 

// Check if Dot has been eaten before and incrementing score

// Check Rows

if (yP == 4) {  // if in Row 1 **********************************************************
  if (xP == 4) { // dot 1
     if (dot[1] == 1) {  // Check if dot gobbled already
        dot[1] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 28) { // dot 2
     if (dot[2] == 1) {  // Check if dot gobbled already
        dot[2] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 52) { // dot 3
     if (dot[3] == 1) {  // Check if dot gobbled already
        dot[3] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 74) { // dot 4
     if (dot[4] == 1) {  // Check if dot gobbled already
        dot[4] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 98) { // dot 5
     if (dot[5] == 1) {  // Check if dot gobbled already
        dot[5] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 6
     if (dot[6] == 1) {  // Check if dot gobbled already
        dot[6] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 168) { // dot 7
     if (dot[7] == 1) {  // Check if dot gobbled already
        dot[7] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 192) { // dot 8
     if (dot[8] == 1) {  // Check if dot gobbled already
        dot[8] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 216) { // dot 9
     if (dot[9] == 1) {  // Check if dot gobbled already
        dot[9] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 238) { // dot 10
     if (dot[10] == 1) {  // Check if dot gobbled already
        dot[10] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 262) { // dot 11
     if (dot[11] == 1) {  // Check if dot gobbled already
        dot[11] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 12
     if (dot[12] == 1) {  // Check if dot gobbled already
        dot[12] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 

} else 
if (yP == 26) {  // if in Row 2  **********************************************************
  if (xP == 4) { // dot 13
     if (dot[13] == 1) {  // Check if dot gobbled already
        dot[13] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score 
        // Turn Ghost Blue if Pacman eats Big Dots
        fruiteatenpacman = true; // Turn Ghost blue      
     }     
  } else
  if (xP == 62) { // dot 14
     if (dot[14] == 1) {  // Check if dot gobbled already
        dot[14] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 15
     if (dot[15] == 1) {  // Check if dot gobbled already
        dot[15] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 168) { // dot 16
     if (dot[16] == 1) {  // Check if dot gobbled already
        dot[16] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 228) { // dot 17
     if (dot[17] == 1) {  // Check if dot gobbled already
        dot[17] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 18
     if (dot[18] == 1) {  // Check if dot gobbled already
        dot[18] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score
        // Turn Ghost Blue if Pacman eats Big Dots
        fruiteatenpacman = true; // Turn Ghost Blue       
     }     
  } 

} else
if (yP == 46) {  // if in Row 3  **********************************************************
  if (xP == 4) { // dot 19
     if (dot[19] == 1) {  // Check if dot gobbled already
        dot[19] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 28) { // dot 20
     if (dot[20] == 1) {  // Check if dot gobbled already
        dot[20] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 52) { // dot 21
     if (dot[21] == 1) {  // Check if dot gobbled already
        dot[21] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 74) { // dot 22
     if (dot[22] == 1) {  // Check if dot gobbled already
        dot[22] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 98) { // dot 23
     if (dot[23] == 1) {  // Check if dot gobbled already
        dot[23] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 24
     if (dot[24] == 1) {  // Check if dot gobbled already
        dot[24] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 146) { // dot 25
     if (dot[25] == 1) {  // Check if dot gobbled already
        dot[25] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else

  if (xP == 168) { // dot 26
     if (dot[26] == 1) {  // Check if dot gobbled already
        dot[26] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 192) { // dot 27
     if (dot[27] == 1) {  // Check if dot gobbled already
        dot[27] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 216) { // dot 28
     if (dot[28] == 1) {  // Check if dot gobbled already
        dot[28] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 238) { // dot 29
     if (dot[29] == 1) {  // Check if dot gobbled already
        dot[29] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 262) { // dot 30
     if (dot[30] == 1) {  // Check if dot gobbled already
        dot[30] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 31
     if (dot[31] == 1) {  // Check if dot gobbled already
        dot[31] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 

} else
if (yP == 168) {  // if in Row 4  **********************************************************
  if (xP == 4) { // dot 42
     if (dot[42] == 1) {  // Check if dot gobbled already
        dot[42] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 28) { // dot 43
     if (dot[43] == 1) {  // Check if dot gobbled already
        dot[43] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 52) { // dot 44
     if (dot[44] == 1) {  // Check if dot gobbled already
        dot[44] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 74) { // dot 45
     if (dot[45] == 1) {  // Check if dot gobbled already
        dot[45] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 98) { // dot 46
     if (dot[46] == 1) {  // Check if dot gobbled already
        dot[46] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 47
     if (dot[47] == 1) {  // Check if dot gobbled already
        dot[47] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 146) { // dot 48
     if (dot[48] == 1) {  // Check if dot gobbled already
        dot[48] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else

  if (xP == 168) { // dot 49
     if (dot[49] == 1) {  // Check if dot gobbled already
        dot[49] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 192) { // dot 50
     if (dot[50] == 1) {  // Check if dot gobbled already
        dot[50] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 216) { // dot 51
     if (dot[51] == 1) {  // Check if dot gobbled already
        dot[51] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 238) { // dot 52
     if (dot[52] == 1) {  // Check if dot gobbled already
        dot[52] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 262) { // dot 53
     if (dot[53] == 1) {  // Check if dot gobbled already
        dot[53] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 54
     if (dot[54] == 1) {  // Check if dot gobbled already
        dot[54] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 

} else
if (yP == 188) {  // if in Row 5  **********************************************************
  if (xP == 4) { // dot 55
     if (dot[55] == 1) {  // Check if dot gobbled already
        dot[55] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score
         // Turn Ghost Blue if Pacman eats Big Dots
        fruiteatenpacman = true; // Turn Ghost blue         
     }     
  } else
  if (xP == 62) { // dot 56
     if (dot[56] == 1) {  // Check if dot gobbled already
        dot[56] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 57
     if (dot[57] == 1) {  // Check if dot gobbled already
        dot[57] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 168) { // dot 58
     if (dot[58] == 1) {  // Check if dot gobbled already
        dot[58] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 228) { // dot 59
     if (dot[59] == 1) {  // Check if dot gobbled already
        dot[59] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 60
     if (dot[60] == 1) {  // Check if dot gobbled already
        dot[60] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score 
          // Turn Ghost Blue if Pacman eats Big Dots
        fruiteatenpacman = true; // Turn Ghost blue        
     }     
  } 

} else
if (yP == 208) {  // if in Row 6  **********************************************************
  if (xP == 4) { // dot 61
     if (dot[61] == 1) {  // Check if dot gobbled already
        dot[61] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 28) { // dot 62
     if (dot[62] == 1) {  // Check if dot gobbled already
        dot[62] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 52) { // dot 63
     if (dot[63] == 1) {  // Check if dot gobbled already
        dot[63] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 74) { // dot 64
     if (dot[64] == 1) {  // Check if dot gobbled already
        dot[64] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 98) { // dot 65
     if (dot[65] == 1) {  // Check if dot gobbled already
        dot[65] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 120) { // dot 66
     if (dot[66] == 1) {  // Check if dot gobbled already
        dot[66] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 168) { // dot 67
     if (dot[67] == 1) {  // Check if dot gobbled already
        dot[67] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 192) { // dot 68
     if (dot[68] == 1) {  // Check if dot gobbled already
        dot[68] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 216) { // dot 69
     if (dot[69] == 1) {  // Check if dot gobbled already
        dot[69] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 238) { // dot 70
     if (dot[70] == 1) {  // Check if dot gobbled already
        dot[70] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 262) { // dot 71
     if (dot[71] == 1) {  // Check if dot gobbled already
        dot[71] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (xP == 284) { // dot 72
     if (dot[72] == 1) {  // Check if dot gobbled already
        dot[72] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 

}   



// Check Columns


if (xP == 28) {  // if in Column 2
  if (yP == 66) { // dot 32
     if (dot[32] == 1) {  // Check if dot gobbled already
        dot[32] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 86) { // dot 34
     if (dot[34] == 1) {  // Check if dot gobbled already
        dot[34] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 108) { // dot 36
     if (dot[36] == 1) {  // Check if dot gobbled already
        dot[36] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 126) { // dot 38
     if (dot[38] == 1) {  // Check if dot gobbled already
        dot[38] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 146) { // dot 40
     if (dot[40] == 1) {  // Check if dot gobbled already
        dot[40] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 

} else
if (xP == 262) {  // if in Column 7
  if (yP == 66) { // dot 33
     if (dot[33] == 1) {  // Check if dot gobbled already
        dot[33] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 86) { // dot 35
     if (dot[35] == 1) {  // Check if dot gobbled already
        dot[35] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 106) { // dot 37
     if (dot[37] == 1) {  // Check if dot gobbled already
        dot[37] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 126) { // dot 39
     if (dot[39] == 1) {  // Check if dot gobbled already
        dot[39] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } else
  if (yP == 146) { // dot 41
     if (dot[41] == 1) {  // Check if dot gobbled already
        dot[41] = 0; // Reset flag to Zero
        pacmanscore++; // Increment pacman score       
     }     
  } 
}

//Pacman wandering Algorithm 
// Note: Keep horizontal and vertical coordinates even numbers only to accomodate increment rate and starting point
// Pacman direction variable D where 0 = right, 1 = down, 2 = left, 3 = up

//****************************************************************************************************************************
//Right hand motion and ***************************************************************************************************
//****************************************************************************************************************************



if(D == 0){
// Increment xP and then test if any decisions required on turning up or down
  xP = xP+cstep; 

 // There are four horizontal rows that need rules

  // First Horizontal Row
  if (yP == 4) { 

    // Past first block decide to continue or go down
    if (xP == 62) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past second block only option is down
    if (xP == 120) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past third block decide to continue or go down
    if (xP == 228) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past fourth block only option is down
    if (xP == 284) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }

  // 2nd Horizontal Row
  if (yP == 46) { 

    // Past upper doorway on left decide to continue or go down
    if (xP == 28) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past first block decide to continue or go up
    if (xP == 62) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Past Second block decide to continue or go up
    if (xP == 120) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Past Mid Wall decide to continue or go up
    if (xP == 168) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past third block decide to continue or go up
    if (xP == 228) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past last clock digit decide to continue or go down
    if (xP == 262) { 
      direct = random(2); // generate random number between 0 and 2
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past fourth block only option is up
    if (xP == 284) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }

  // LHS Door Horizontal Row
  if (yP == 108) { 

    // Past upper doorway on left decide to go up or go down
    if (xP == 28) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 3;}    
    }
  }

  // 3rd Horizontal Row
  if (yP == 168) { 

    // Past lower doorway on left decide to continue or go up
    if (xP == 28) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past first block decide to continue or go down
    if (xP == 62) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Past Second block decide to continue or go down
    if (xP == 120) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Past Mid Wall decide to continue or go down
    if (xP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past third block decide to continue or go down
    if (xP == 228) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past last clock digit decide to continue or go up
    if (xP == 262) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past fourth block only option is down
    if (xP == 284) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }
 
  
  // 4th Horizontal Row
  if (yP == 208) { 

    // Past first block decide to continue or go up
    if (xP == 62) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past second block only option is up
    if (xP == 120) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past third block decide to continue or go up
    if (xP == 228) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past fourth block only option is up
    if (xP == 284) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
   }
}

//****************************************************************************************************************************
//Left hand motion **********************************************************************************************************
//****************************************************************************************************************************

else if(D == 2){
// Increment xP and then test if any decisions required on turning up or down
  xP = xP-cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xP,80,165); // Print xP
  myGLCD->drawString(yP,110,165); // Print yP
*/

 // There are four horizontal rows that need rules

  // First Horizontal Row  ******************************
  if (yP == 4) { 

     // Past first block only option is down
    if (xP == 4) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Past second block decide to continue or go down
    if (xP == 62) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past third block only option is down
    if (xP == 168) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past fourth block decide to continue or go down
    if (xP == 228) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
  }

  // 2nd Horizontal Row ******************************
  if (yP == 46) { 

    // Meet LHS wall only option is up
    if (xP == 4) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Meet upper doorway on left decide to continue or go down
    if (xP == 28) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet first block decide to continue or go up
    if (xP == 62) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Meet Second block decide to continue or go up
    if (xP == 120) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Meet Mid Wall decide to continue or go up
    if (xP == 168) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet third block decide to continue or go up
    if (xP == 228) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet last clock digit decide to continue or go down
    if (xP == 262) { 
      direct = random(2); // generate random number between 0 and 3
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

  }
  

  // 3rd Horizontal Row ******************************
  if (yP == 168) { 

    // Meet LHS lower wall only option is down
    if (xP == 4) { 
         D = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }

    // Meet lower doorway on left decide to continue or go up
    if (xP == 28) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet first block decide to continue or go down
    if (xP == 62) { 
      direct = random(2); // generate random number between 0 and 3
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Meet Second block decide to continue or go down
    if (xP == 120) { 
      direct = random(2); // generate random number between 0 and 3
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Meet Mid Wall decide to continue or go down
    if (xP == 168) { 
      direct = random(2); // generate random number between 0 and 3
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet third block decide to continue or go down
    if (xP == 228) { 
      direct = random(2); // generate random number between 0 and 3
      if (direct == 1){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet last clock digit above decide to continue or go up
    if (xP == 262) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    
    }
    
  }
   // 4th Horizontal Row ******************************
  if (yP == 208) { 

    // Meet LHS wall only option is up
    if (xP == 4) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }  
    // Meet first block decide to continue or go up
    if (xP == 62) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Meet bottom divider wall only option is up
    if (xP == 168) { 
         D = 3; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Meet 3rd block decide to continue or go up
    if (xP == 228) { 
      direct = random(4); // generate random number between 0 and 3
      if (direct == 3){
         D = direct; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
   
  
  }
}  
  


//****************************************************************************************************************************
//Down motion **********************************************************************************************************
//****************************************************************************************************************************

else if(D == 1){
// Increment yP and then test if any decisions required on turning up or down
  yP = yP+cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xP,80,165); // Print xP
  myGLCD->drawString(yP,110,165); // Print yP
*/

 // There are vertical rows that need rules

  // First Vertical Row  ******************************
  if (xP == 4) { 

     // Past first block only option is right
    if (yP == 46) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yP == 208) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  }

  // 2nd Vertical Row ******************************
  if (xP == 28) { 

    // Meet bottom doorway on left decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 3rd Vertical Row ******************************
  if (xP == 62) { 

    // Meet top lh digit decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet top lh digit decide to go left or go right
    if (yP == 208) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 5th Vertical Row ******************************
  if (xP == 120) { 

    // Meet top lh digit decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet bottom wall only opgion to go left
    if (yP == 208) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 6th Vertical Row ******************************
  if (xP == 168) { 

    // Meet top lh digit decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet bottom wall only opgion to go right
    if (yP == 208) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 8th Vertical Row ******************************
  if (xP == 228) { 

    // Meet top lh digit decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet bottom wall
    if (yP == 208) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 9th Vertical Row ******************************
  if (xP == 262) { 

    // Meet bottom right doorway  decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 10th Vertical Row  ******************************
  if (xP == 284) { 

     // Past first block only option is left
    if (yP == 46) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yP == 208) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  } 
}  

//****************************************************************************************************************************
//Up motion **********************************************************************************************************
//****************************************************************************************************************************

else if(D == 3){
// Decrement yP and then test if any decisions required on turning up or down
  yP = yP-cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xP,80,165); // Print xP
  myGLCD->drawString(yP,110,165); // Print yP
*/


 // There are vertical rows that need rules

  // First Vertical Row  ******************************
  if (xP == 4) { 

     // Past first block only option is right
    if (yP == 4) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yP == 168) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  }

  // 2nd Vertical Row ******************************
  if (xP == 28) { 

    // Meet top doorway on left decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 3rd Vertical Row ******************************
  if (xP == 62) { 

    // Meet top lh digit decide to go left or go right
    if (yP == 4) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet top lh digit decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 5th Vertical Row ******************************
  if (xP == 120) { 

    // Meet bottom lh digit decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet top wall only opgion to go left
    if (yP == 4) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 6th Vertical Row ******************************
  if (xP == 168) { 

    // Meet bottom lh digit decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet top wall only opgion to go right
    if (yP == 4) { 
         D = 0; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 8th Vertical Row ******************************
  if (xP == 228) { 

    // Meet bottom lh digit decide to go left or go right
    if (yP == 168) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }

    // Meet top wall go left or right
    if (yP == 4) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 9th Vertical Row ******************************
  if (xP == 262) { 

    // Meet top right doorway  decide to go left or go right
    if (yP == 46) { 
      direct = random(2); // generate random number between 0 and 1
      if (direct == 1){
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { D = 0;}    
    }
  }

  // 10th Vertical Row  ******************************
  if (xP == 284) { 

     // Past first block only option is left
    if (yP == 168) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards top wall only option right
    if (yP == 4) { 
         D = 2; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  } 
 }  
}

}




void displayghost(){ // Draw Ghost in position on screen
//******************************************************************************************************************
//Ghost ; 
// Note: Keep horizontal and verticalcoordinates even numbers only to accomodateincrement rate and starting point
// Ghost direction variable  D where 0 = right, 1 = down, 2 = left, 3 = up

//****************************************************************************************************************************
//Right hand motion **********************************************************************************************************
//****************************************************************************************************************************


// If ghost captured then ghost dissapears until reset
if ((fruiteatenpacman == true)&&(abs(xG-xP)<=5)&&(abs(yG-yP)<=5)){ 
  
  if (ghostlost == false){
    pacmanscore++;
    pacmanscore++;  
  }

  ghostlost = true;

  dly = gamespeed; // slowdown now only drawing one item
  }
  
  
if (ghostlost == false){ // only draw ghost if still alive

drawGhost(xG,yG,GD,prevGD); // Draws Ghost at these coordinates


// If Ghost is on a dot then print the adjacent dots if they are valid

//  myGLCD->setColor(200, 200, 200);
  
// Check Rows

if (yG == 4) {  // if in Row 1 **********************************************************
  if (xG == 4) { // dot 1
     if (dot[2] == 1) {  // Check if dot 2 gobbled already
    myGLCD->fillCircle(42, 19, 2, TFT_GREY); // dot 2
     }    
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     }    

  } else
  if (xG == 28) { // dot 2
     if (dot[1] == 1) {  // Check if dot 1 gobbled already
    myGLCD->fillCircle(19, 19, 2, TFT_GREY); // dot 1
     }    
      if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }    

  } else
  if (xG == 52) { // dot 3
     if (dot[2] == 1) {  // Check if dot 2 gobbled already
    myGLCD->fillCircle(42, 19, 2, TFT_GREY); // dot 2
     }    
      if (dot[4] == 1) {  // Check if dot 4 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }   
  } else
  if (xG == 74) { // dot 4
     if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }    
      if (dot[5] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(112, 19, 2, TFT_GREY); // dot 5
     }   
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }    
  } else
  if (xG == 98) { // dot 5
     if (dot[4] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     }    
      if (dot[6] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(136, 19, 2, TFT_GREY); // dot 6
     }     
  } else
  if (xG == 120) { // dot 6
     if (dot[5] == 1) {  // Check if dot 5 gobbled already
    myGLCD->fillCircle(136, 19, 2, TFT_GREY); // dot 5
     }    
      if (dot[15] == 1) {  // Check if dot 15 gobbled already
    myGLCD->fillCircle(136, 40, 2, TFT_GREY); // dot 15
     }     
  } else
 

 if (xG == 168) { // dot 7
      if (dot[16] == 1) {  // Check if dot 16 gobbled already
    myGLCD->fillCircle(183, 40, 2, TFT_GREY); // dot 16
     }    
      if (dot[8] == 1) {  // Check if dot 8 gobbled already
    myGLCD->fillCircle(206, 19, 2, TFT_GREY); // dot 8
     }     
  } else
  if (xG == 192) { // dot 8
      if (dot[7] == 1) {  // Check if dot 7 gobbled already
    myGLCD->fillCircle(183, 19, 2, TFT_GREY); // dot 7
     }    
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }    
  } else
  if (xG == 216) { // dot 9
      if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }    
      if (dot[8] == 1) {  // Check if dot 8 gobbled already
    myGLCD->fillCircle(206, 19, 2, TFT_GREY); // dot 8
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
 } else
  if (xG == 238) { // dot 10
      if (dot[11] == 1) {  // Check if dot 11 gobbled already
    myGLCD->fillCircle(275, 19, 2, TFT_GREY); // dot 11
     }    
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
  } else
  if (xG == 262) { // dot 11
      if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }    
      if (dot[12] == 1) {  // Check if dot 12 gobbled already
    myGLCD->fillCircle(298, 19, 2, TFT_GREY); // dot 12
     }    
      if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
     } 
  } else
  if (xG == 284) { // dot 12
      if (dot[11] == 1) {  // Check if dot 11 gobbled already
    myGLCD->fillCircle(275, 19, 2, TFT_GREY); // dot 11
     }    
      if (dot[18] == 1) {  // Check if dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
     }  
  }
} else 
if (yG == 26) {  // if in Row 2  **********************************************************
  if (xG == 4) { // dot 13
     if (dot[1] == 1) {  // Check if dot 1 gobbled already
    myGLCD->fillCircle(19, 19, 2, TFT_GREY); // dot 1
     }    
      if (dot[19] == 1) {  // Check if dot 19 gobbled already
    myGLCD->fillCircle(19, 60, 2, TFT_GREY); //  dot 19
     }   
  } else
  
    if (xG == 62) { // dot 14
      if (dot[3] == 1) {  // Check if dot 3 gobbled already
    myGLCD->fillCircle(65, 19, 2, TFT_GREY); // dot 3
     }   
         if (dot[4] == 1) {  // Check if dot 4 gobbled already
    myGLCD->fillCircle(88, 19, 2, TFT_GREY); // dot 4
     } 
         if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }   
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     }    
     
  } else
  
  if (xG == 120) { // dot 15
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[6] == 1) {  // Check if dot 6 gobbled already
    myGLCD->fillCircle(136, 19, 2, TFT_GREY); // dot 6
     }      
  } else
  if (xG == 168) { // dot 16
      if (dot[7] == 1) {  // Check if dot 7 gobbled already
    myGLCD->fillCircle(183, 19, 2, TFT_GREY); // dot 7
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }          
  } else
    if (xG == 228) { // dot 17
      if (dot[9] == 1) {  // Check if dot 9 gobbled already
    myGLCD->fillCircle(229, 19, 2, TFT_GREY); // dot 9
     }      
       if (dot[10] == 1) {  // Check if dot 10 gobbled already
    myGLCD->fillCircle(252, 19, 2, TFT_GREY); // dot 10
     }  
      if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }  
       if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }     
     
  } else
  if (xG == 284) { // dot 18
      if (dot[31] == 1) {  // Check if dot 31 gobbled already
    myGLCD->fillCircle(298, 60, 2, TFT_GREY); // dot 31
     }    
      if (dot[12] == 1) {  // Check if dot 12 gobbled already
    myGLCD->fillCircle(298, 19, 2, TFT_GREY); // dot 12
     }  
  }
} else
if (yG == 46) {  // if in Row 3  **********************************************************
  if (xG == 4) { // dot 19
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }    
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     }  
  } else
  if (xG == 28) { // dot 20
     if (dot[19] == 1) {  // Check if dot 19 gobbled already
    myGLCD->fillCircle(19, 60, 2, TFT_GREY); // dot 19
     }    
      if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }   
      if (dot[32] == 1) {  // Check if dot 32 gobbled already
    myGLCD->fillCircle(42, 80, 2, TFT_GREY); // dot 32
     }
      if (dot[13] == 1) {  // Check if dot 13 gobbled already
    myGLCD->fillCircle(19, 40, 7, TFT_GREY); // Big dot 13
     }     
  } else
  if (xG == 52) { // dot 21
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }    
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }         
  } else
  if (xG == 74) { // dot 22
      if (dot[21] == 1) {  // Check if dot 21 gobbled already
    myGLCD->fillCircle(65, 60, 2, TFT_GREY); // dot 21
     }    
      if (dot[23] == 1) {  // Check if dot 23 gobbled already
    myGLCD->fillCircle(112, 60, 2, TFT_GREY); // dot 23
     } 
      if (dot[14] == 1) {  // Check if dot 14 gobbled already
    myGLCD->fillCircle(77, 40, 2, TFT_GREY); // dot 14
     }    
  } else
  if (xG == 98) { // dot 23
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[22] == 1) {  // Check if dot 22 gobbled already
    myGLCD->fillCircle(88, 60, 2, TFT_GREY); // dot 22
     }  
    
  } else
  if (xG == 120) { // dot 24
      if (dot[25] == 1) {  // Check if dot 25 gobbled already
    myGLCD->fillCircle(160, 60, 2, TFT_GREY); // dot 25
     }    
      if (dot[23] == 1) {  // Check if dot 23 gobbled already
    myGLCD->fillCircle(112, 60, 2, TFT_GREY); // dot 23
     }
      if (dot[15] == 1) {  // Check if dot 15 gobbled already
    myGLCD->fillCircle(136, 40, 2, TFT_GREY); // dot 15
     }        
  } else
  if (xG == 146) { // dot 25
     if (dot[24] == 1) {  // Check if dot 24 gobbled already
    myGLCD->fillCircle(136, 60, 2, TFT_GREY); // dot 24
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }    
  } else
  if (xG == 168) { // dot 26
      if (dot[25] == 1) {  // Check if dot 25 gobbled already
    myGLCD->fillCircle(160, 60, 2, TFT_GREY); // dot 25
     }    
      if (dot[27] == 1) {  // Check if dot 27 gobbled already
    myGLCD->fillCircle(206, 60, 2, TFT_GREY); // dot 27
     }
      if (dot[16] == 1) {  // Check if dot 16 gobbled already
    myGLCD->fillCircle(183, 40, 2, TFT_GREY); // dot 16
     }    
  } else
  if (xG == 192) { // dot 27
     if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }    
      if (dot[26] == 1) {  // Check if dot 26 gobbled already
    myGLCD->fillCircle(183, 60, 2, TFT_GREY); // dot 26
     }      
  } else
  if (xG == 216) { // dot 28
      if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }    
      if (dot[27] == 1) {  // Check if dot 27 gobbled already
    myGLCD->fillCircle(206, 60, 2, TFT_GREY); // dot 27
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
  } else
  if (xG == 238) { // dot 29
     if (dot[28] == 1) {  // Check if dot 28 gobbled already
    myGLCD->fillCircle(229, 60, 2, TFT_GREY); // dot 28
     }    
      if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
     }      
      if (dot[17] == 1) {  // Check if dot 17 gobbled already
    myGLCD->fillCircle(241, 40, 2, TFT_GREY); // dot 17
     }   
   
  } else
  if (xG == 262) { // dot 30
      if (dot[29] == 1) {  // Check if dot 29 gobbled already
    myGLCD->fillCircle(252, 60, 2, TFT_GREY); // dot 29
     }    
      if (dot[33] == 1) {  // Check if dot 33 gobbled already
    myGLCD->fillCircle(275, 80, 2, TFT_GREY); // dot 33
     }      
      if (dot[31] == 1) {  // Check if dot 31 gobbled already
    myGLCD->fillCircle(298, 60, 2, TFT_GREY); // dot 31
     }  
   if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
   }  
  } else
  if (xG == 284) { // dot 31
   if (dot[18] == 1) {  // Check if Big dot 18 gobbled already
    myGLCD->fillCircle(298, 40, 7, TFT_GREY); // dot 18
   }     
   if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
   } 
  }
} else

if (yG == 168) {  // if in Row 4  **********************************************************
  if (xG == 4) { // dot 42
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }     
     if (dot[55] == 1) {  // Check if dot 55 gobbled already
    myGLCD->fillCircle(19, 201, 7, TFT_GREY); // dot 55
     }     
  } else
  if (xG == 28) { // dot 43
     if (dot[42] == 1) {  // Check if dot 42 gobbled already
    myGLCD->fillCircle(19, 181, 2, TFT_GREY); // dot 42
     }     
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     }   
      if (dot[40] == 1) {  // Check if dot 40 gobbled already
    myGLCD->fillCircle(42, 160, 2, TFT_GREY); // dot 40
     }       
  } else
  if (xG == 52) { // dot 44
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }     
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     } 
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }    
  } else
  if (xG == 74) { // dot 45
     if (dot[46] == 1) {  // Check if dot 46 gobbled already
    myGLCD->fillCircle(112, 181, 2, TFT_GREY); // dot 46
     }     
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     } 
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }    
     
  } else
  if (xG == 98) { // dot 46
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     }  
  } else
  if (xG == 120) { // dot 47
     if (dot[48] == 1) {  // Check if dot 48 gobbled already
    myGLCD->fillCircle(160, 181, 2, TFT_GREY); // dot 48
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
     }     
     if (dot[46] == 1) {  // Check if dot 46 gobbled already
    myGLCD->fillCircle(112, 181, 2, TFT_GREY); // dot 46
     } 
     if (dot[57] == 1) {  // Check if dot 57 gobbled already
    myGLCD->fillCircle(136, 201, 2, TFT_GREY); // dot 57 
     }
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    } 
  } else
  if (xG == 146) { // dot 48
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }  
  } else

  if (xG == 168) { // dot 49
     if (dot[48] == 1) {  // Check if dot 48 gobbled already
    myGLCD->fillCircle(160, 181, 2, TFT_GREY); // dot 48
     }     
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
     if (dot[50] == 1) {  // Check if dot 50 gobbled already
    myGLCD->fillCircle(206, 181, 2, TFT_GREY); // dot 50
     } 
     if (dot[58] == 1) {  // Check if dot 58 gobbled already
    myGLCD->fillCircle(183, 201, 2, TFT_GREY); // dot 58
     }        
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  if (xG == 192) { // dot 50
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }      
  } else
  if (xG == 216) { // dot 51
     if (dot[50] == 1) {  // Check if dot 50 gobbled already
    myGLCD->fillCircle(206, 181, 2, TFT_GREY); // dot 50
     }    
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }     
  } else
  if (xG == 238) { // dot 52
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }    
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }  
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }     
  } else
 if (xG == 262) { // dot 53
     if (dot[41] == 1) {  // Check if dot 41 gobbled already
    myGLCD->fillCircle(275, 160, 2, TFT_GREY); // dot 41
     }    
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[54] == 1) {  // Check if dot 54 gobbled already
    myGLCD->fillCircle(298, 181, 2, TFT_GREY); // dot 54
     } 
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }         
  } else
  if (xG == 284) { // dot 54
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }    
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }      
  } 

} else
if (yG == 188) {  // if in Row 5  **********************************************************
  if (xG == 4) { // dot 55
     if (dot[42] == 1) {  // Check if dot 42 gobbled already
    myGLCD->fillCircle(19, 181, 2, TFT_GREY); // dot 42
     } 
     if (dot[61] == 1) {  // Check if dot 61 gobbled already
    myGLCD->fillCircle(19, 221, 2, TFT_GREY); // dot 61
     }    
  } else
   if (xG == 62) { // dot 56
     if (dot[44] == 1) {  // Check if dot 44 gobbled already
    myGLCD->fillCircle(65, 181, 2, TFT_GREY); // dot 44
     } 
     if (dot[45] == 1) {  // Check if dot 45 gobbled already
    myGLCD->fillCircle(88, 181, 2, TFT_GREY); // dot 45
     } 
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     }      
     
  } else
  
  if (xG == 120) { // dot 57
     if (dot[47] == 1) {  // Check if dot 47 gobbled already
    myGLCD->fillCircle(136, 181, 2, TFT_GREY); // dot 47
     }     
     if (dot[66] == 1) {  // Check if dot 66 gobbled already
    myGLCD->fillCircle(136, 221, 2, TFT_GREY); // dot 66
     }    
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  if (xG == 168) { // dot 58
     if (dot[67] == 1) {  // Check if dot 67 gobbled already
    myGLCD->fillCircle(183, 221, 2, TFT_GREY); // dot 67
     }     
     if (dot[49] == 1) {  // Check if dot 49 gobbled already
    myGLCD->fillCircle(183, 181, 2, TFT_GREY); // dot 49
     }       
    // Draw fruit
    if ((fruitdrawn == true)&&(fruitgone == false)){ // draw fruit again
       myGLCD->drawicon(146, 168, fruit); //   draw fruit 
    }
  } else
  
  if (xG == 228) { // dot 59
     if (dot[51] == 1) {  // Check if dot 51 gobbled already
    myGLCD->fillCircle(229, 181, 2, TFT_GREY); // dot 51
     }
     if (dot[52] == 1) {  // Check if dot 52 gobbled already
    myGLCD->fillCircle(252, 181, 2, TFT_GREY); // dot 52
     } 
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     } 
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }      
     
  } else
  
  if (xG == 284) { // dot 60
     if (dot[72] == 1) {  // Check if dot 72 gobbled already
    myGLCD->fillCircle(298, 221, 2, TFT_GREY); //  dot 72
     } 
     if (dot[54] == 1) {  // Check if dot 54 gobbled already
    myGLCD->fillCircle(298, 181, 2, TFT_GREY); // dot 54
     }    
  } 

} else


if (yG == 208) {  // if in Row 6  **********************************************************
  if (xG == 4) { // dot 61
     if (dot[55] == 1) {  // Check if dot 55 gobbled already
    myGLCD->fillCircle(19, 201, 7, TFT_GREY); // dot 55
     } 
     if (dot[62] == 1) {  // Check if dot 62 gobbled already
    myGLCD->fillCircle(42, 221, 2, TFT_GREY); // dot 62
     }   
  } else
  if (xG == 28) { // dot 62
     if (dot[61] == 1) {  // Check if dot 61 gobbled already
    myGLCD->fillCircle(19, 221, 2, TFT_GREY); // dot 61
     }  
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }      
  } else
  if (xG == 52) { // dot 63
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     } 
     if (dot[62] == 1) {  // Check if dot 62 gobbled already
    myGLCD->fillCircle(42, 221, 2, TFT_GREY); // dot 62
     }  
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }      
  } else
  if (xG == 74) { // dot 64
     if (dot[65] == 1) {  // Check if dot 65 gobbled already
    myGLCD->fillCircle(112, 221, 2, TFT_GREY); // dot 65
     } 
     if (dot[63] == 1) {  // Check if dot 63 gobbled already
    myGLCD->fillCircle(65, 221, 2, TFT_GREY); // dot 63
     }  
     if (dot[56] == 1) {  // Check if dot 56 gobbled already
    myGLCD->fillCircle(77, 201, 2, TFT_GREY); // dot 56 
     }     
  } else
  if (xG == 98) { // dot 65
     if (dot[64] == 1) {  // Check if dot 64 gobbled already
    myGLCD->fillCircle(88, 221, 2, TFT_GREY); // dot 64
     } 
     if (dot[66] == 1) {  // Check if dot 66 gobbled already
    myGLCD->fillCircle(136, 221, 2, TFT_GREY); // dot 66
     }    
  } else
  if (xG == 120) { // dot 66
     if (dot[65] == 1) {  // Check if dot 65 gobbled already
    myGLCD->fillCircle(112, 221, 2, TFT_GREY); // dot 65
     } 
     if (dot[57] == 1) {  // Check if dot 57 gobbled already
    myGLCD->fillCircle(136, 201, 2, TFT_GREY); // dot 57 
     }    
  } else
  if (xG == 168) { // dot 67
     if (dot[68] == 1) {  // Check if dot 68 gobbled already
    myGLCD->fillCircle(206, 221, 2, TFT_GREY); // dot 68
     } 
     if (dot[58] == 1) {  // Check if dot 58 gobbled already
    myGLCD->fillCircle(183, 201, 2, TFT_GREY); // dot 58
     }     
  } else
  if (xG == 192) { // dot 68
     if (dot[67] == 1) {  // Check if dot 67 gobbled already
    myGLCD->fillCircle(183, 221, 2, TFT_GREY); // dot 67
     } 
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     }    
  } else
  if (xG == 216) { // dot 69
     if (dot[68] == 1) {  // Check if dot 68 gobbled already
    myGLCD->fillCircle(206, 221, 2, TFT_GREY); // dot 68
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }    
  } else
  if (xG == 238) { // dot 70
     if (dot[69] == 1) {  // Check if dot 69 gobbled already
    myGLCD->fillCircle(229, 221, 2, TFT_GREY); // dot 69
     } 
     if (dot[59] == 1) {  // Check if dot 59 gobbled already
    myGLCD->fillCircle(241, 201, 2, TFT_GREY); // dot 59
     }
     if (dot[71] == 1) {  // Check if dot 71 gobbled already
    myGLCD->fillCircle(275, 221, 2, TFT_GREY); // dot 71
     }       
  } else
  if (xG == 262) { // dot 71
     if (dot[70] == 1) {  // Check if dot 70 gobbled already
    myGLCD->fillCircle(252, 221, 2, TFT_GREY); // dot 70
     }  
     if (dot[72] == 1) {  // Check if dot 72 gobbled already
    myGLCD->fillCircle(298, 221, 2, TFT_GREY); // dot 72
     }       
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }
  } else
  if (xG == 284) { // dot 72
     if (dot[71] == 1) {  // Check if dot 71 gobbled already
    myGLCD->fillCircle(275, 221, 2, TFT_GREY); // dot 71
     } 
     if (dot[60] == 1) {  // Check if dot 60 gobbled already
    myGLCD->fillCircle(298, 201, 7, TFT_GREY); // Big dot 60
     }     
  }
} else



// Check Columns


if (xG == 28) {  // if in Column 2
  if (yG == 66) { // dot 32
     if (dot[20] == 1) {  // Check if dot 20 gobbled already
    myGLCD->fillCircle(42, 60, 2, TFT_GREY); // dot 20
     }     
     if (dot[34] == 1) {  // Check if dot 34 gobbled already
    myGLCD->fillCircle(42, 100, 2, TFT_GREY); // dot 34
     }        
  } else
  if (yG == 86) { // dot 34
      if (dot[32] == 1) {  // Check if dot 32 gobbled already
    myGLCD->fillCircle(42, 80, 2, TFT_GREY); // dot 32
     }  
      if (dot[36] == 1) {  // Check if dot 36 gobbled already
    myGLCD->fillCircle(42, 120, 2, TFT_GREY); // dot 36
     }      
  } else
  if (yG == 106) { // dot 36
     if (dot[38] == 1) {  // Check if dot 38 gobbled already
    myGLCD->fillCircle(42, 140, 2, TFT_GREY); // dot 38
     }     
     if (dot[34] == 1) {  // Check if dot 34 gobbled already
    myGLCD->fillCircle(42, 100, 2, TFT_GREY); // dot 34
     }      
  } else
  if (yG == 126) { // dot 38
      if (dot[40] == 1) {  // Check if dot 40 gobbled already
    myGLCD->fillCircle(42, 160, 2, TFT_GREY); // dot 40
     } 
      if (dot[36] == 1) {  // Check if dot 36 gobbled already
    myGLCD->fillCircle(42, 120, 2, TFT_GREY); // dot 36
     }       
  } else
  if (yG == 146) { // dot 40
     if (dot[38] == 1) {  // Check if dot 38 gobbled already
    myGLCD->fillCircle(42, 140, 2, TFT_GREY); // dot 38
     }     
     if (dot[43] == 1) {  // Check if dot 43 gobbled already
    myGLCD->fillCircle(42, 181, 2, TFT_GREY); // dot 43
     }      
  } 

} else
if (xG == 262) {  // if in Column 7

  if (yG == 66) { // dot 33
      if (dot[30] == 1) {  // Check if dot 30 gobbled already
    myGLCD->fillCircle(275, 60, 2, TFT_GREY); // dot 30
     }   
      if (dot[35] == 1) {  // Check if dot 35 gobbled already
    myGLCD->fillCircle(275, 100, 2, TFT_GREY); // dot 35
     }   
  } else
  if (yG == 86) { // dot 35
      if (dot[33] == 1) {  // Check if dot 33 gobbled already
    myGLCD->fillCircle(275, 80, 2, TFT_GREY); // dot 33
     }  
      if (dot[37] == 1) {  // Check if dot 37 gobbled already
    myGLCD->fillCircle(275, 120, 2, TFT_GREY); // dot 37
     }     
  } else
  if (yG == 106) { // dot 37
      if (dot[35] == 1) {  // Check if dot 35 gobbled already
    myGLCD->fillCircle(275, 100, 2, TFT_GREY); // dot 35
     }  
      if (dot[39] == 1) {  // Check if dot 39 gobbled already
    myGLCD->fillCircle(275, 140, 2, TFT_GREY); // dot 39
     }      
  } else
  if (yG == 126) { // dot 39
      if (dot[37] == 1) {  // Check if dot 37 gobbled already
    myGLCD->fillCircle(275, 120, 2, TFT_GREY); // dot 37
     }
     if (dot[41] == 1) {  // Check if dot 41 gobbled already
    myGLCD->fillCircle(275, 160, 2, TFT_GREY); // dot 41
     }       
  } else
  if (yG == 146) { // dot 41
      if (dot[39] == 1) {  // Check if dot 39 gobbled already
    myGLCD->fillCircle(275, 140, 2, TFT_GREY); // dot 39
     } 
     if (dot[53] == 1) {  // Check if dot 53 gobbled already
    myGLCD->fillCircle(275, 181, 2, TFT_GREY); // dot 53
     }     
  } 
}




// Capture legacy direction to enable adequate blanking of trail
prevGD = GD;

if(GD == 0){
// Increment xG and then test if any decisions required on turning up or down
  xG = xG+cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xG,80,165); // Print xG
  myGLCD->drawString(yP,110,165); // Print yP
*/



 // There are four horizontal rows that need rules

  // First Horizontal Row
  if (yG== 4) { 

    // Past first block decide to continue or go down
    if (xG == 62) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past second block only option is down
    if (xG == 120) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past third block decide to continue or go down
    if (xG == 228) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past fourth block only option is down
    if (xG == 284) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }

  // 2nd Horizontal Row
  if (yG == 46) { 

    // Past upper doorway on left decide to continue right or go down
    if (xG == 28) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past first block decide to continue right or go up
    if (xG == 62) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }
     // Past Second block decide to continue right or go up
    if (xG == 120) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }

     // Past Mid Wall decide to continue right or go up
    if (xG == 168) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }

    // Past third block decide to continue right or go up
    if (xG == 228) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }

    // Past last clock digit decide to continue or go down
    if (xG == 262) { 
      gdirect = random(2); // generate random number between 0 and 2
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past fourth block only option is up
    if (xG == 284) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }

  // 3rd Horizontal Row
  if (yG== 168) { 

    // Past lower doorway on left decide to continue right or go up
    if (xG == 28) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }

    // Past first block decide to continue or go down
    if (xG == 62) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Past Second block decide to continue or go down
    if (xG == 120) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Past Mid Wall decide to continue or go down
    if (xG == 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past third block decide to continue or go down
    if (xG == 228) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Past last clock digit decide to continue right or go up
    if (xG == 262) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }

    // Past fourth block only option is down
    if (xG == 284) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
  }
 
  
  // 4th Horizontal Row
  if (yG== 208) { 

    // Past first block decide to continue right or go up
    if (xG == 62) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }
    // Past second block only option is up
    if (xG == 120) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past third block decide to continue right or go up
    if (xG == 228) { 
      if (random(2) == 0){
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 3;}
    }
    // Past fourth block only option is up
    if (xG == 284) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
   }
}

//****************************************************************************************************************************
//Left hand motion **********************************************************************************************************
//****************************************************************************************************************************

else if(GD == 2){
// Increment xG and then test if any decisions required on turning up or down
  xG = xG-cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xG,80,165); // Print xG
  myGLCD->drawString(yP,110,165); // Print yP
*/

 // There are four horizontal rows that need rules

  // First Horizontal Row  ******************************
  if (yG== 4) { 

     // Past first block only option is down
    if (xG == 4) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Past second block decide to continue or go down
    if (xG == 62) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
    // Past third block only option is down
    if (xG == 168) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Past fourth block decide to continue or go down
    if (xG == 228) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
  }

  // 2nd Horizontal Row ******************************
  if (yG== 46) { 

    // Meet LHS wall only option is up
    if (xG == 4) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Meet upper doorway on left decide to continue left or go down
    if (xG == 28) { 
      if (random(2) == 0){
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }

    // Meet first block decide to continue left or go up
    if (xG == 62) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }
     // Meet Second block decide to continue left or go up
    if (xG == 120) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }

     // Meet Mid Wall decide to continue left or go up
    if (xG == 168) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }

    // Meet third block decide to continue left or go up
    if (xG == 228) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }

    // Meet last clock digit decide to continue or go down
    if (xG == 262) { 
      gdirect = random(2); // generate random number between 0 and 3
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

  }
 
   // RHS Door Horizontal Row
  if (yG == 108) { 

    // Past upper doorway on left decide to go up or go down
    if (xG == 262) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 1; // set Pacman direciton varialble to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 3;}    
    }
  } 

  // 3rd Horizontal Row ******************************
  if (yG== 168) { 

    // Meet LHS lower wall only option is down
    if (xG == 4) { 
         GD = 1; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }

    // Meet lower doorway on left decide to continue left or go up
    if (xG == 28) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }

    // Meet first block decide to continue or go down
    if (xG == 62) { 
      gdirect = random(2); // generate random number between 0 and 3
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }
     // Meet Second block decide to continue or go down
    if (xG == 120) { 
      gdirect = random(2); // generate random number between 0 and 3
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

     // Meet Mid Wall decide to continue or go down
    if (xG == 168) { 
      gdirect = random(2); // generate random number between 0 and 3
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet third block decide to continue or go down
    if (xG == 228) { 
      gdirect = random(2); // generate random number between 0 and 3
      if (gdirect == 1){
         GD = gdirect; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      }
    }

    // Meet last clock digit above decide to continue left or go up
    if (xG == 262) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    
    }
    
  }
   // 4th Horizontal Row ******************************
  if (yG== 208) { 

    // Meet LHS wall only option is up
    if (xG == 4) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }  
    // Meet first block decide to continue left or go up
    if (xG == 62) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }
    // Meet bottom divider wall only option is up
    if (xG == 168) { 
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
    // Meet 3rd block decide to continue left or go up
    if (xG == 228) { 
      if (random(2) == 0){
         GD = 3; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } else { GD = 2;}
    }
   
  
  }
}  
  


//****************************************************************************************************************************
//Down motion **********************************************************************************************************
//****************************************************************************************************************************

else if(GD == 1){
// Increment yGand then test if any decisions required on turning up or down
  yG= yG+cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xG,80,165); // Print xG
  myGLCD->drawString(yP,110,165); // Print yP
*/

 // There are vertical rows that need rules

  // First Vertical Row  ******************************
  if (xG == 4) { 

     // Past first block only option is right
    if (yG== 46) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yG== 208) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  }

  // 2nd Vertical Row ******************************
  if (xG == 28) { 

    // Meet bottom doorway on left decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 3rd Vertical Row ******************************
  if (xG == 62) { 

    // Meet top lh digit decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet top lh digit decide to go left or go right
    if (yG== 208) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 5th Vertical Row ******************************
  if (xG == 120) { 

    // Meet top lh digit decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet bottom wall only opgion to go left
    if (yG== 208) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 6th Vertical Row ******************************
  if (xG == 168) { 

    // Meet top lh digit decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet bottom wall only opgion to go right
    if (yG== 208) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 8th Vertical Row ******************************
  if (xG == 228) { 

    // Meet top lh digit decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet bottom wall
    if (yG== 208) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 9th Vertical Row ******************************
  if (xG == 262) { 

    // Meet bottom right doorway  decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 10th Vertical Row  ******************************
  if (xG == 284) { 

     // Past first block only option is left
    if (yG== 46) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yG== 208) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  } 
}  

//****************************************************************************************************************************
//Up motion **********************************************************************************************************
//****************************************************************************************************************************

else if(GD == 3){
// Decrement yGand then test if any decisions required on turning up or down
  yG= yG-cstep; 

/* Temp print variables for testing
  
  myGLCD->setColor(0, 0, 0);
  myGLCD->setBackColor(114, 198, 206);
  myGLCD->drawString(xG,80,165); // Print xG
  myGLCD->drawString(yP,110,165); // Print yP
*/


 // There are vertical rows that need rules

  // First Vertical Row  ******************************
  if (xG == 4) { 

     // Past first block only option is right
    if (yG== 4) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards bottom wall only option right
    if (yG== 168) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  }

  // 2nd Vertical Row ******************************
  if (xG == 28) { 

    // Meet top doorway on left decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 3rd Vertical Row ******************************
  if (xG == 62) { 

    // Meet top lh digit decide to go left or go right
    if (yG== 4) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet top lh digit decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 5th Vertical Row ******************************
  if (xG == 120) { 

    // Meet bottom lh digit decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet top wall only opgion to go left
    if (yG== 4) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 6th Vertical Row ******************************
  if (xG == 168) { 

    // Meet bottom lh digit decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet top wall only opgion to go right
    if (yG== 4) { 
         GD = 0; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up   
    }
  }

  // 8th Vertical Row ******************************
  if (xG == 228) { 

    // Meet bottom lh digit decide to go left or go right
    if (yG== 168) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }

    // Meet top wall go left or right
    if (yG== 4) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 9th Vertical Row ******************************
  if (xG == 262) { 

    // Meet top right doorway  decide to go left or go right
    if (yG== 46) { 
      gdirect = random(2); // generate random number between 0 and 1
      if (gdirect == 1){
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
      } 
      else { GD = 0;}    
    }
  }

  // 10th Vertical Row  ******************************
  if (xG == 284) { 

     // Past first block only option is left
    if (yG== 168) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    }
 
    // Towards top wall only option right
    if (yG== 4) { 
         GD = 2; // set Ghost direction variable to new direction D where 0 = right, 1 = down, 2 = left, 3 = up
    } 
  } 
}  

}
  




//******************************************************************************************************************

//******************************************************************************************************************



/*
//temp barriers

if (yP>200) {
  yP=46;
}
if(xP>260){
  xP=4;
}
*/
}







//***************************************************************************************************** 
//====== Draws the Pacman - bitmap
//*****************************************************************************************************
void drawPacman(int x, int y, int p, int d, int pd) {



  // Draws the Pacman - bitmap
//  // Pacman direction d == 0 = right, 1 = down, 2 = left, 3 = up

if ( d == 0){ // Right

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

if (p == 0) { 


   if (mspacman == false){
   myGLCD->drawicon(x, y, c_pacman); //   Closed Pacman  
   } else {
   myGLCD->drawicon(x, y, ms_c_pacman_r); //   Closed Pacman        
   }


 } else if( p == 1) {

   if (mspacman == false){
  myGLCD->drawicon(x, y, r_m_pacman); //  Medium open Pacman 
   } else {
  myGLCD->drawicon(x, y, ms_r_m_pacman); //  Medium open Pacman       
   }
   
 } else if( p == 2) {

   if (mspacman == false){
  myGLCD->drawicon(x, y, r_o_pacman); //  Open Mouth Pacman  
   } else {
  myGLCD->drawicon(x, y, ms_r_o_pacman); //  Open Mouth Pacman       
   }
 }
} else  if ( d == 1){ // Down

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

 if (p == 0) { 
   
   if (mspacman == false){
   myGLCD->drawicon(x, y, c_pacman); //   Closed Pacman  
   } else {
   myGLCD->drawicon(x, y, ms_c_pacman_d); //   Closed Pacman        
   }
    
    
 } else if( p == 1) {

   if (mspacman == false){
  myGLCD->drawicon(x, y, d_m_pacman); //  Medium open Pacman   
   } else {
  myGLCD->drawicon(x, y, ms_d_m_pacman); //  Medium open Pacman     
   }

 } else if( p == 2) {

   if (mspacman == false){
    myGLCD->drawicon(x, y, d_o_pacman); //  Open Mouth Pacman  
   } else {
    myGLCD->drawicon(x, y, ms_d_o_pacman); //  Open Mouth Pacman     
   }

 }
} else  if ( d == 2){ // Left

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

 if (p == 0) { 

   if (mspacman == false){
   myGLCD->drawicon(x, y, c_pacman); //   Closed Pacman  
   } else {
   myGLCD->drawicon(x, y, ms_c_pacman_l); //   Closed Pacman        
   }


 } else if( p == 1) {

   if (mspacman == false){
    myGLCD->drawicon(x, y, l_m_pacman); //  Medium open Pacman   
   } else {
    myGLCD->drawicon(x, y, ms_l_m_pacman); //  Medium open Pacman   
   }
   
 } else if( p == 2) {
 
   if (mspacman == false){
    myGLCD->drawicon(x, y, l_o_pacman); //  Open Mouth Pacman   
   } else {
    myGLCD->drawicon(x, y, ms_l_o_pacman); //  Open Mouth Pacman  
   }

 }
} else  if ( d == 3){ // Up

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

 if (p == 0) { 

   if (mspacman == false){
   myGLCD->drawicon(x, y, c_pacman); //   Closed Pacman  
   } else {
   myGLCD->drawicon(x, y, ms_c_pacman_u); //   Closed Pacman        
   }


 } else if( p == 1) {

   if (mspacman == false){
    myGLCD->drawicon(x, y, u_m_pacman); //  Medium open Pacman    
   } else {
    myGLCD->drawicon(x, y, ms_u_m_pacman); //  Medium open Pacman   
   }
   

 } else if( p == 2) {

   if (mspacman == false){
    myGLCD->drawicon(x, y, u_o_pacman); //  Open Mouth Pacman    
   } else {
    myGLCD->drawicon(x, y, ms_u_o_pacman); //  Open Mouth Pacman  
   }
   
 }

}
  
}

//**********************************************************************************************************
//====== Draws the Ghost - bitmap
void drawGhost(int x, int y, int d, int pd) {


  // Draws the Ghost - bitmap
//  // Ghost direction d == 0 = right, 1 = down, 2 = left, 3 = up


if ( d == 0){ // Right

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

  if (fruiteatenpacman == true){ 
   myGLCD->drawicon(x, y, bluepacman); //   Closed Ghost 
  } else {
   myGLCD->drawicon(x, y, rr_ghost); //   Closed Ghost 
  }
  
} else  if ( d == 1){ // Down

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

  if (fruiteatenpacman == true){ 
   myGLCD->drawicon(x, y, bluepacman); //   Closed Ghost 
  } else {
   myGLCD->drawicon(x, y, rd_ghost); //   Closed Ghost 
  }

} else  if ( d == 2){ // Left

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

  if (fruiteatenpacman == true){ 
   myGLCD->drawicon(x, y, bluepacman); //   Closed Ghost 
  } else {
   myGLCD->drawicon(x, y, rl_ghost); //   Closed Ghost 
  }

} else  if ( d == 3){ // Up

if (pd == 0){ // Legacy direction Right
  myGLCD->fillRect(x-1, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new position
  }
if (pd == 3){ // Legacy direction Up
 myGLCD->fillRect(x, y+28, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position
}
if (pd == 1){ // Legacy direction Down
 myGLCD->fillRect(x, y-1, 28, 2, TFT_BLACK); // Clear trail off graphic before printing new position 
}
if (pd == 2){ // Legacy direction Left
 myGLCD->fillRect(x+28, y, 2, 28, TFT_BLACK); // Clear trail off graphic before printing new positi 
}

     if (fruiteatenpacman == true){ 
     myGLCD->drawicon(x, y, bluepacman); //   Closed Ghost 
    } else {
     myGLCD->drawicon(x, y, ru_ghost); //   Closed Ghost 
    }

  }
  
}

