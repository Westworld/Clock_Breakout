#ifndef pacman_
#define pacman_

#include "Screen.h"

#define byte uint8_t

#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define TFT_GREY 0xC618

#include "pacman/bluepacman.h"
#include "pacman/fruit.h"
#include "pacman/ms_c_pacman_l.h"
#include "pacman/ms_d_o_pacman.h"
#include "pacman/ms_r_o_pacman.h"
#include "pacman/r_o_pacman.h"
#include "pacman/ru_ghost.h"
#include "pacman/c_pacman.h"
#include "pacman/l_m_pacman.h"
#include "pacman/ms_c_pacman_r.h"
#include "pacman/ms_l_m_pacman.h"
#include "pacman/ms_u_m_pacman.h"
#include "pacman/rd_ghost.h"
#include "pacman/u_m_pacman.h"
#include "pacman/d_m_pacman.h"
#include "pacman/l_o_pacman.h"
#include "pacman/ms_c_pacman_u.h"
#include "pacman/ms_l_o_pacman.h"
#include "pacman/ms_u_o_pacman.h"
#include "pacman/rl_ghost.h"
#include "pacman/u_o_pacman.h"
#include "pacman/d_o_pacman.h"
#include "pacman/ms_c_pacman_d.h"
#include "pacman/ms_d_m_pacman.h"
#include "pacman/ms_r_m_pacman.h"
#include "pacman/r_m_pacman.h"
#include "pacman/rr_ghost.h"

void pacman_init(Screen * mastertft) ;
void pacman_run();
void setgamespeed(short userspeedsetting);

void drawscreen();
void UpdateDisp();
void printscoreboard();
void drawfruit(); 
void refreshgame();
void displaypacman();
void displayghost();

void drawPacman(int x, int y, int p, int d, int pd);
void drawGhost(int x, int y, int d, int pd);
#endif