#include <SPI.h>
#include <Adafruit_GFX.h>
#include "TFT_ST7735.h"
#include <VoltageReference.h>
#include <OneButton.h>
#include <EEPROM.h>
#include "sec_setup.h"
#include "melodies.h"

//#define _DEBUG_

//#define THREE_BUTTON

VoltageReference vRef;

#define BUTTON_START_PIN A1
#define BUTTON_LIST_PIN  A2
#define BUTTON_MODE_PIN  A3

OneButton btnStart(BUTTON_START_PIN, true);
OneButton btnList(BUTTON_LIST_PIN, true);
#ifdef TREE_BUTTON
OneButton btnMode(BUTTON_MODE_PIN, true);
#endif

#define __CS 10           // TFT CS
#define __DC 8            // TFT A0
#define __RES 9           // TFT RESET

TFT_ST7735 display = TFT_ST7735(__CS, __DC, __RES);

unsigned long workTimer;
unsigned long voltTimer;
unsigned long beepTimer = 0;
unsigned int deciSec = 0;
unsigned int deciList[30];    // Stored results
unsigned char listCnt = 0;
unsigned char listIdx = 0;

params_t params;
params_t paramsSaved;
params_t paramsEdit;

int correctTimer;

unsigned int santiV;
char cell = 0;                // Battery cells count 0 - 1S

unsigned char startLong = 0;  // Start long pressed
unsigned char setupMode = 0;  // Setup mode 0 - Off, 1 - Menu, 2 - Edit param
unsigned int offCount = 0;  // For sleep mode if low voltage
unsigned long offOn = 0;     // For sleep mode beeper
char menuNumb = 0;
char menuItem = 0;
char menuPos = 0;
char menuEnter = 0;
char menuEsc = 0;

// ==================================================================================
void setup() {
  display.begin();
  randomSeed(analogRead(A4)*millis());
  random(melodyCnt);
  btnStart.attachClick(clStart);
  btnStart.attachLongPressStart(lsStart);
  btnStart.attachLongPressStop(ltStart);
  btnList.attachClick(clList);
  btnList.attachLongPressStart(lsList);
#ifdef TREE_BUTTON
  btnMode.attachClick(clMode);
  btnMode.attachDoubleClick(dcMode);
  btnMode.attachLongPressStart(lsMode);
#endif

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIN_TONE, OUTPUT);

  vRef.begin();
  // analogReference(INTERNAL);
  analogReference(DEFAULT);
  /////////////////////////////////////////
  //  manual EEPROM reset
  /////////////////////////////////////////
  //      EEPROM.update(0, 0xFF);
  /////////////////////////////////////////

  eeprom(); // Read / set initial values

  /////////////////////////////////////////
  //  manual EEPROM edit
  /////////////////////////////////////////
  //    params.correctTimer = 45;
  //    EEPROM.put( 1, params);
  /////////////////////////////////////////
  
  correctTimer = (params.correctTimer == 0) ? 0 : TIMER_CORRECT / params.correctTimer;

#ifdef _DEBUG_
  Serial.begin(115200);
  while (!Serial);
#endif

  logo();
  santiV = getBatt(10);
  cell = (santiV > BATT_2S) ? 2 : 1;
  startUp();
  workTimer = 0;
  voltTimer = millis() + DELAY_VOLT;

}
// ----------------------------------------------------------------------------------
void loop() {
  btnStart.tick();
  btnList.tick();
#ifdef TREE_BUTTON
  btnMode.tick();
#endif

  unsigned long curMillis = millis();
  if (beepTimer && (beepTimer < curMillis)) {
    beepTimer = 0;
    digitalWrite(BUZZER_PIN, LOW);
    noTone(PIN_TONE);
  }
  if ((offCount > OFF_CNT) && !offOn) {
    beepTimer = curMillis + OFF_DUR;
    offOn = curMillis + OFF_DELAY;
    tone(PIN_TONE, pgm_read_word_near(notesFq + ((random(2)) ? params.toneOn : params.toneOff)));
  }
  if (offOn && (offOn < curMillis)) {
    offOn = 0;
  }
  if (workTimer && (workTimer < curMillis)) {
    workTimer += DELAY_TIMER;
    deciSec++;
    if (correctTimer && (deciSec % abs(correctTimer) == 0)) {
      if (correctTimer > 0) {
        workTimer++;
      }
      else {
        workTimer--;
      }
    }
    dispSec();
    if (params.dur_15) {   // 15 seconds beep enabled
      if (deciSec % 600 == 0) {
        beepTimer = curMillis + params.dur_15;
        tone(PIN_TONE, pgm_read_word_near(notesFq + params.tone_60));
      } else if (deciSec % 150 == 0) {
        beepTimer = curMillis + params.dur_15;
        tone(PIN_TONE, pgm_read_word_near(notesFq + params.tone_15));
      }
    }
  } else {
    if (voltTimer < curMillis) {
      voltTimer += DELAY_VOLT;
      dispVolt();
    }
    if (setupMode) {  // Setup enabled
      if (menuPos != 0) { // + or - pressed
        if (setupMode == 1) {
          setupMove(menuPos);
        } else { // Edit mode
          editRow(menuPos);
        }
        menuPos = 0;
      } // menuPos != 0
      if (menuEnter) {
        if (setupMode == 1) {
          if (menuNumb == 0) {
            switch (menuItem) {
              case 0: // Exit
                break;
              case 1: // Save
                EEPROM.put( 1, params);
                break;
              case 2: // Restore
                params = paramsSaved;
                break;
              case 3: // Default
                paramsDefault();
                eeprom(1);
                break;
              default:
                break;
            }
            setupMode = 0;
            startUp();
          } else {
            setupMode = 2;
            paramsEdit = params;
            setupRow(menuItem);
          }
        } else { //setupMode == 2 - exit from line edit
          setupMode = 1;
          setupRow(menuItem);
        }
        menuEnter = 0;
      } // menuEnter
      if (menuEsc) {
        if (setupMode == 1) {
          setupMode = 0;
          startUp();
        } else { //setupMode == 2 - exit from line edit
          setupMode = 1;
          setupRow(menuItem);
        }
        menuEsc = 0;
      } // menuEsc
    }
  }
}
// ==================================================================================

void clStart() {
  if (setupMode) {
    menuPos = 1;
    return;
  }
  digitalWrite(BUZZER_PIN, HIGH);
  beepTimer = millis() + params.durStartStop;
  if (workTimer) {
    tone(PIN_TONE, pgm_read_word_near(notesFq + params.toneOff));
    workTimer = 0;
    deciList[listCnt] = deciSec;
    deciSec = 0;
    dispSec();
    dispRow(listCnt++);
  } else {
    workTimer = millis() + DELAY_TIMER;
    tone(PIN_TONE, pgm_read_word_near(notesFq + params.toneOn));
  }
}
void lsStart() {
  if (workTimer) return;
  if (setupMode) {
    menuEnter = 1;
    return;
  }
  startLong = 1;
}
void ltStart() {
  if (workTimer) return;
  startLong = 0;
}
void clList() {
  if (workTimer) return;
  if (setupMode) {
    menuPos = -1;
    return;
  }
  if (listCnt <= 5) return;
  for (char i = listIdx; i < listIdx + 5; i++) {
    dispRow(i);
  }
  listIdx = ((listIdx + 5) < listCnt) ? listIdx + 5 : 0;
}
void lsList() {
  if (workTimer) return;
  if (setupMode) {
    menuEsc = 1;
    return;
  }
  if (startLong) { // Set setup mode
    setupMode = 1;
    menuNumb = 0;
    menuItem = 0;
    paramsSaved = params;
    setupDisp();
  } else { // Clear flight list
    listCnt = 0;
    listIdx = 0;
    startUp();
  }
}

#ifdef TREE_BUTTON
void clMode() {
  if (workTimer) return;
  colorTr++;
  if (params.colorTr >= colorsCnt) params.colorTr = 0;
  startUp();
}
void dcMode() {
  if (workTimer) return;
  colorBg++;
  if (params.colorBg >= colorsCnt) params.colorBg = 0;
  startUp();
}
void lsMode() {
  if (workTimer) return;
  params.colorFg++;
  if (params.colorFg >= colorsCnt) colorFg = 0;
  startUp();
}
#endif
