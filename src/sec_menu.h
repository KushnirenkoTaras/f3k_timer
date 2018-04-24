#ifndef _SEC_MENU_H_
#define _SEC_MENU_H_

#include <avr/pgmspace.h>

const char menuStr_0[]  PROGMEM = "Setup";
const char menuStr_00[] PROGMEM = "Exit";
const char menuStr_01[] PROGMEM = "Save";
const char menuStr_02[] PROGMEM = "Restore";
const char menuStr_03[] PROGMEM = "Default";

const char menuStr_1[]  PROGMEM = "Color";
const char menuStr_10[] PROGMEM = "Backgr";
const char menuStr_11[] PROGMEM = "Timer";
const char menuStr_12[] PROGMEM = "Results";

const char menuStr_2[]  PROGMEM = "Tone";
const char menuStr_20[] PROGMEM = "Start";
const char menuStr_21[] PROGMEM = "Stop";
const char menuStr_22[] PROGMEM = "15 sec";
const char menuStr_23[] PROGMEM = "Minute";

const char menuStr_3[]  PROGMEM = "Batt";
const char menuStr_30[] PROGMEM = "Off ";
const char menuStr_31[] PROGMEM = "Crit";
const char menuStr_32[] PROGMEM = "Warn";

const char menuStr_4[]  PROGMEM = "Time";
const char menuStr_40[] PROGMEM = "Corr";
const char menuStr_41[] PROGMEM = "Start";
const char menuStr_42[] PROGMEM = "Beep";

//typedef 
struct menuItem_t {
  const byte itemCnt;
  const char *title;
  const char **items;
};

const char *items_0[] = {menuStr_00, menuStr_01, menuStr_02, menuStr_03};
const char *items_1[] = {menuStr_10, menuStr_11, menuStr_12};
const char *items_2[] = {menuStr_20, menuStr_21, menuStr_22, menuStr_23};
const char *items_3[] = {menuStr_30, menuStr_31, menuStr_32};
const char *items_4[] = {menuStr_40, menuStr_41, menuStr_42};

const menuItem_t setupMenu[] = {
  {sizeof(items_0)/sizeof(int), menuStr_0, &items_0[0]},
  {sizeof(items_1)/sizeof(int), menuStr_1, &items_1[0]},
  {sizeof(items_2)/sizeof(int), menuStr_2, &items_2[0]},
  {sizeof(items_3)/sizeof(int), menuStr_3, &items_3[0]},
  {sizeof(items_4)/sizeof(int), menuStr_4, &items_4[0]},
};

const unsigned int menuCnt = sizeof(setupMenu) / sizeof(menuItem_t);

#endif
