#include "sec_setup.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "TFT_ST7735.h"
#include "melodies.h"
#include "sec_menu.h"

// music
void playMusic (unsigned char idx ) {
  melody_t curMelody;
  memcpy_P(&curMelody, &melodies[idx], sizeof(melody_t));
  note_t *notes = (note_t *)malloc(sizeof(note_t) * curMelody.noteNumb);
  memcpy_P (&notes[0], (PGM_P)pgm_read_word(&(melodies[idx].melody)), sizeof(note_t)*curMelody.noteNumb);

  for (size_t i = 0; i < curMelody.noteNumb; i++) { // Цикл от 0 до количества нот
    tone(PIN_TONE, notes[i].freq); // Включаем звук, определенной частоты
    delay(notes[i].dur * curMelody.temp);  // Дауза для заданой ноты
    noTone(PIN_TONE); // Останавливаем звук
  }
  digitalWrite(PIN_TONE, LOW);
  free(notes);
} // music /

void logo() { // Logo screen
  display.fillScreen(colors[params.colorBg]);
  display.drawRect(0, 0, 128, 128, GLCD_CL_GOLD);
  display.setTextSize(3);
  display.setCursor(27, 7);
  display.setTextColor(colors[params.colorFg]);
  display.print("F3k");
  display.setCursor(10, 37);
  display.print("Timer");
  display.setCursor(42, 72);
  display.print("by");
  display.setCursor(15, 102);
  display.print("Taras");
#ifdef _DEBUG_
  Serial.print(analogRead(3));
  Serial.print(" ");
  Serial.print(random(melodyCnt));
  Serial.print(" ");
  Serial.println(melodyCnt);
#endif
  playMusic(random(melodyCnt));
} // Logo screen

void eeprom(unsigned char write) {
  if (EEPROM.read(0) == 13) { // EEPROM initialized - read
    EEPROM.get(1, params);
  } else { // EEPROM not initialized - write default values
    write = 1;
    paramsDefault();
  }
  if (write) {
    EEPROM.update(0, 13);
    EEPROM.put( 1, params);
  }
}

void paramsDefault() {
  params = {
    300,  //  unsigned int battOff;
    350,  //  unsigned int battCrit;
    380,  //  unsigned int battWarn;
    200,  //  unsigned int durStartStop;
    50,   //  unsigned int dur_15;
    0,    //  int correctTimer;
    0,    //  unsigned char colorBg;
    1,    //  unsigned char colorTr;
    2,    //  unsigned char colorFg;
    51,   //  unsigned char toneOn;
    63,   //  unsigned char toneOff;
    75,   //  unsigned char tone_15;
    87,   //  unsigned char tone_60;
  };
}; // paramsDefault

int getBatt(int scale) { // if 1 - result in mV 12345 Set 100 for sV 12.6
  long curVoltage = 0;
  for (size_t i = 0; i < DIV_CNT; i++) {
    curVoltage += analogRead(DETECT_PIN);
    delay(5);
  }
  int vcc = vRef.readVcc();
#ifdef _DEBUG_
  Serial.print (curVoltage);
  Serial.print ("|");
  Serial.println (vcc);
  display.setTextSize(2);
  display.setCursor(0, 99);
  char buf[30];
  sprintf(buf, "%d %d", vcc, curVoltage);
  display.print(buf);
#endif
#ifdef BUTTON_2S_BATT
  return curVoltage * vcc * (DIV_R1 + DIV_R2) / DIV_R2 / DIV_CNT / 1023 / scale;
#else
  return curVoltage * vcc / DIV_CNT / 1023 / scale;
#endif
} // getBatt

void dispVolt() {
  santiV = getBatt(10);
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (cell) {
    display.setTextColor(colors[params.colorFg], colors[params.colorBg]);
    char buf[20];
    if (santiV >= cell * params.battWarn) {
      display.setTextColor(GLCD_CL_GREEN, colors[params.colorBg]);
    } else if (santiV < cell * params.battCrit) {
      display.setTextColor(GLCD_CL_RED, colors[params.colorBg]);
      if (santiV < cell * params.battOff) {
        offCount++;
      } else {
        offCount = 0;
      }
    } else {
      display.setTextColor(GLCD_CL_YELLOW, colors[params.colorBg]);
    }
    sprintf(buf, "Battery %dS %d.%02dV", cell, santiV / 100, santiV % 100);
    display.print(buf);
  } else {
    display.setTextColor(GLCD_CL_RED, colors[params.colorBg]);
    display.print("Battery unknown");
  }
} // dispVolt

// Show saved time
void dispRow(unsigned char row) {
  display.setTextSize(2);
  display.setCursor(0, 41 + (row % 5) * 18);
  if (row >= listCnt) {
    display.print("          ");
  } else {
    display.setTextColor(colors[params.colorFg], colors[params.colorBg]);
    // display.setTextColor(RED);
    unsigned int min = deciList[row] / 600;
    unsigned int sec = deciList[row] % 600;
    unsigned int dSec = sec % 10;
    sec /= 10;
    char buf[12];
    sprintf(buf, "%02d %02d:%02d.%1d", row + 1, min, sec, dSec);
    display.print(buf);
  }
} // dispRow

void dispSec() {// Show timer
  display.setTextSize(3);
  display.setCursor(0, 11);
  display.setTextColor(colors[params.colorTr], colors[params.colorBg]);
  // display.setTextColor(RED);
//  unsigned int min = deciSec / 600;
  unsigned int sec = deciSec % 600;
//  unsigned int dSec = sec % 10;
//  sec /= 10;
  char buf[10];
//  sprintf(buf, "%02d:%02d.%1d", min, sec, dSec);
  sprintf(buf, "%02d:%02d.%1d", deciSec / 600, sec / 10, sec % 10);
  display.print(buf);
}

void startUp() {
  display.fillScreen(colors[params.colorBg]);
  dispVolt();
  dispSec();
  for (char i = 0; i < listCnt; i++) {
    dispRow(i);
  }
}
////////////////////////////////////////////////////////
// Menu func
////////////////////////////////////////////////////////

void setupRow(unsigned char row) {
  char buf[13];
  char out[13];
  //  char buf[13] = "1234567890";
  strcpy_P(buf, setupMenu[menuNumb].items[row]);
  int param;
  char prefix;
  prefix = (row == menuItem) ? ((setupMode == 1) ? '>' : '*') : ' ';
  switch (menuNumb) {
    case 3: // Batt
      switch (row) {
        case 0: // Off
          param = params.battOff;
          break;
        case 1: // Crit
          param = params.battCrit;
          break;
        case 2: // Warn
          param = params.battWarn;
          break;
        default:
          param = 666;
          break;
      }
      sprintf(out, "%c%s %1d.%02d", prefix, buf, param / 100, param % 100);
      break;
    case 4: // Time
      switch (row) {
        case 0: // correctTimer
          param = params.correctTimer;
          break;
        case 1: // durStartStop
          param = params.durStartStop;
          break;
        case 2: // dur_15
          param = params.dur_15;
          break;
        default:
          break;
      }
      sprintf(out, "%c%s %03d", prefix, buf, param);
      break;
    default:
      sprintf(out, "%c%s", prefix, buf);
      break;
  }
  display.setTextSize(2);
  display.setCursor(0, 41 + (row % 5) * 18);
//  display.setTextColor(colors[params.colorFg], (setupMode == 1) ? colors[params.colorBg] : GLCD_CL_DARK_BLUE);
  display.setTextColor(colors[params.colorFg], colors[params.colorBg]);
  display.print(out);
#ifdef _DEBUG_
  Serial.println(buf);
#endif

} // setupRow

void editRow(char menuPos) {
  switch (menuNumb) {
    case 1: // Color
      switch (menuItem) {
        case 0: // Bg Color
          params.colorBg = ((params.colorBg == 0) && (menuPos < 0)) ? 0 : (((params.colorBg == (colorsCnt - 1)) && (menuPos > 0)) ? colorsCnt - 1 : params.colorBg + menuPos);
          break;
        case 1: // Time Color
          params.colorTr = ((params.colorTr == 0) && (menuPos < 0)) ? 0 : (((params.colorTr == (colorsCnt - 1)) && (menuPos > 0)) ? colorsCnt - 1 : params.colorTr + menuPos);
          break;
        case 2: // List Color
          params.colorFg = ((params.colorFg == 0) && (menuPos < 0)) ? 0 : (((params.colorFg == (colorsCnt - 1)) && (menuPos > 0)) ? colorsCnt - 1 : params.colorFg + menuPos);
          break;
        default:
          break;
      }
      setupDisp();
      break;
    case 2: // Tone
      switch (menuItem) {
        case 0: // Start
          params.toneOn = ((params.toneOn == 0) && (menuPos < 0)) ? 0 : (((params.toneOn == (NOTES_CNT - 1)) && (menuPos > 0)) ? NOTES_CNT - 1 : params.toneOn + menuPos);
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.toneOn));
          break;
        case 1: // Stop
          params.toneOff = ((params.toneOff == 0) && (menuPos < 0)) ? 0 : (((params.toneOff == (NOTES_CNT - 1)) && (menuPos > 0)) ? NOTES_CNT - 1 : params.toneOff + menuPos);
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.toneOff));
          break;
        case 2: // 15 sec
          params.tone_15 = ((params.tone_15 == 0) && (menuPos < 0)) ? 0 : (((params.tone_15 == (NOTES_CNT - 1)) && (menuPos > 0)) ? NOTES_CNT - 1 : params.tone_15 + menuPos);
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.tone_15));
          break;
        case 3: // 60 sec
          params.tone_60 = ((params.tone_60 == 0) && (menuPos < 0)) ? 0 : (((params.tone_60 == (NOTES_CNT - 1)) && (menuPos > 0)) ? NOTES_CNT - 1 : params.tone_60 + menuPos);
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.tone_60));
          break;
        default:
          break;
      }
      beepTimer = millis() + (menuItem < 2) ? params.durStartStop : params.dur_15;
      setupRow(menuItem);
      break;
    case 3: // Batt
      switch (menuItem) {
        case 0: // Off
          if ((params.battOff > 200) && (params.battOff < 500)) params.battOff += menuPos;
          break;
        case 1: // Crit
          if ((params.battCrit > 250) && (params.battCrit < 500)) params.battCrit += menuPos;
          break;
        case 2: // Warn
          if ((params.battWarn > 250) && (params.battWarn < 500)) params.battWarn += menuPos;
          break;
        default:
          break;
      }
      setupRow(menuItem);
      break;
    case 4: // Time
      switch (menuItem) {
        case 0: // Corr
          params.correctTimer += menuPos;
          correctTimer = (params.correctTimer == 0) ? 0 : TIMER_CORRECT / params.correctTimer;
          break;
        case 1: // Start
          if ((params.durStartStop > 50) && (params.durStartStop < 500)) params.durStartStop += menuPos * 10;
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.toneOn));
          beepTimer = millis() + params.durStartStop;
          break;
        case 2: // Beep
          if ((params.dur_15 > 0) && (params.dur_15 < 500)) params.dur_15 += menuPos * 10;
          tone(PIN_TONE, pgm_read_word_near(notesFq + params.tone_15));
          beepTimer = millis() + params.dur_15;
          break;
        default:
          break;
      }
      setupRow(menuItem);
      break;
    default:
      return;
  }
}

void setupMove(char menuPos) {
  if (menuPos == -1) {
    if (menuItem == 0) {
      if (menuNumb == 0) {
        menuNumb = menuCnt - 1;
      } else {
        menuNumb--;
      }
      menuItem = setupMenu[menuNumb].itemCnt - 1;
      setupDisp();
    } else {
      menuItem--;
      setupRow(menuItem);
      setupRow(menuItem + 1);
    }
  } else if (menuPos == 1) {
    menuItem++;
    if (menuItem >= setupMenu[menuNumb].itemCnt) {
      menuItem = 0;
      menuNumb++;
      if (menuNumb >= menuCnt) {
        menuNumb = 0;
      }
      setupDisp();
    } else {
      setupRow(menuItem);
      setupRow(menuItem - 1);
    }
  }
}

void setupDisp() {
  char buf[13];
  display.fillScreen(colors[params.colorBg]);
  dispVolt();
#ifdef _DEBUG_
  Serial.println("Setup");
#endif
  display.setTextSize(2);
  display.setCursor(0, 11);
  display.setTextColor(colors[params.colorTr], colors[params.colorBg]);
  display.print("-C");
  display.setCursor(102, 11);
  display.print("E+");
  display.setCursor(33, 11);
  //  menuNumb = 4;
  strcpy_P(buf, (char*)setupMenu[menuNumb].title);
  display.print(buf);
  for (size_t i = 0; i < setupMenu[menuNumb].itemCnt; i++) {
    setupRow(i);
  }
} // setupDisp

