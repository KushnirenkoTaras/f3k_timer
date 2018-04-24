#ifndef _SEC_SETUP_H_
#define _SEC_SETUP_H_

#include "TFT_ST7735.h"
#include <VoltageReference.h>
#include <avr/pgmspace.h>
#include  <avr/sleep.h>

extern TFT_ST7735 display;
extern VoltageReference vRef;

struct params_t {
  unsigned int battOff;
  unsigned int battCrit;
  unsigned int battWarn;

  unsigned int durStartStop;
  unsigned int dur_15;
  int correctTimer;

  unsigned char colorBg;
  unsigned char colorTr;
  unsigned char colorFg;
  
  unsigned char toneOn;
  unsigned char toneOff;
  unsigned char tone_15;
  unsigned char tone_60;
};

extern params_t params;
extern params_t paramsSaved;
extern unsigned int santiV;
extern char cell;

extern unsigned long beepTimer;
extern int correctTimer;

extern unsigned int deciSec;
extern unsigned int deciList[30];
extern unsigned char listCnt;
extern unsigned char listIdx;

extern unsigned char setupMode;
extern unsigned int offCount;
extern char menuNumb;
extern char menuItem;

void logo();
void paramsDefault();
void eeprom(unsigned char write = 0);
int getBatt(int scale = 1);
void dispVolt();
void dispRow(unsigned char row);
void dispSec();
void startUp();
void setupRow(unsigned char row);
void editRow(char menuPos);
void setupMove(char menuPos);
void setupDisp();

//#define BUTTON_2S_BATT    // Uncomment if used 2S battery & voltage divider 
#define BUZZER_PIN  A4      // Buzzer port
#define PIN_TONE  7         // Speaker port 

#define DETECT_PIN A0       // Check voltage port
#define BATT_2S 500

#define DIV_R1  1           // Voltage resistor 1
#define DIV_R2  1           // Voltage resistor 2
#define DIV_CNT 3           // Meashuring count

#define DELAY_TIMER 100     // Timer resolution in ms - 0.1 second
#define TIMER_CORRECT 600   // Timer correct to 1 minutes
#define DELAY_VOLT 5000     // Check battery voltage every 5 seconds

#define OFF_CNT 3          // Low battery voltage check count before beep
#define OFF_DUR 70         // Low battery voltage beep duration
#define OFF_DELAY 5000     // Low battery voltage interval

#define GLCD_CL_BLACK 0x0000
#define GLCD_CL_WHITE 0xFFFF
#define GLCD_CL_GRAY 0x7BEF
#define GLCD_CL_LIGHT_GRAY 0xC618
#define GLCD_CL_GREEN 0x07E0
#define GLCD_CL_LIME 0x87E0
#define GLCD_CL_BLUE 0x001F
#define GLCD_CL_DARK_BLUE 0x0009
#define GLCD_CL_RED 0xF800
#define GLCD_CL_AQUA 0x5D1C
#define GLCD_CL_YELLOW 0xFFE0
#define GLCD_CL_MAGENTA 0xF81F
#define GLCD_CL_CYAN 0x07FF
#define GLCD_CL_DARK_CYAN 0x03EF
#define GLCD_CL_ORANGE 0xFCA0
#define GLCD_CL_PINK 0xF97F
#define GLCD_CL_BROWN 0x8200
#define GLCD_CL_VIOLET 0x9199
#define GLCD_CL_SILVER 0xA510
#define GLCD_CL_GOLD 0xA508
#define GLCD_CL_NAVY 0x000F
#define GLCD_CL_MAROON 0x7800
#define GLCD_CL_PURPLE 0x780F
#define GLCD_CL_OLIVE 0x7BE0

const unsigned int colors[] = {
  GLCD_CL_BLACK,     GLCD_CL_WHITE,  GLCD_CL_YELLOW,    GLCD_CL_RED,        GLCD_CL_GREEN, // 0-3
  GLCD_CL_DARK_BLUE, GLCD_CL_BLUE,   GLCD_CL_GRAY,      GLCD_CL_LIGHT_GRAY,
  GLCD_CL_GREEN,   GLCD_CL_LIME,     GLCD_CL_AQUA,
  GLCD_CL_MAGENTA, GLCD_CL_CYAN,   GLCD_CL_DARK_CYAN, GLCD_CL_ORANGE, GLCD_CL_PINK,
  GLCD_CL_BROWN,   GLCD_CL_VIOLET, GLCD_CL_SILVER,    GLCD_CL_GOLD,   GLCD_CL_NAVY,
  GLCD_CL_MAROON,  GLCD_CL_PURPLE, GLCD_CL_OLIVE,
};
const unsigned char colorsCnt = sizeof(colors)/sizeof(int);

#endif
