#ifndef _MELODIES_
#define _MELODIES_

#include "pitches.h"

struct note_t {
  int freq;   // Note frequency
  int dur;    // Note duration
};

const int TEMP_TONE = 9;

const note_t melImperMarch[] PROGMEM = {
  {NOTE_G4,  DUR_1_4},
  {NOTE_G4,  DUR_1_4},
  {NOTE_G4,  DUR_1_4},
  {NOTE_DS4, DUR_1_8P},
  {NOTE_AS4, DUR_1_16},

  {NOTE_G4,  DUR_1_4},
  {NOTE_DS4, DUR_1_8P},
  {NOTE_AS4, DUR_1_16},
  {NOTE_G4,  DUR_1_2},
};
const note_t melProschSl[] PROGMEM = {
  {NOTE_C5,  DUR_1_4},
  {NOTE_GS4, DUR_1_4},
  {NOTE_G4,  DUR_1_4},
  {NOTE_F4,  DUR_1_4},

  {NOTE_DS4, DUR_1_8},
  {NOTE_G4,  DUR_1_8},
  {NOTE_D4,  DUR_1_8},
  {NOTE_DS4, DUR_1_8},
  {NOTE_C4,  DUR_1_4},
};
const note_t melOyNaGori[] PROGMEM = {
  {NOTE_G5,  DUR_1_4},
  {NOTE_D5,  DUR_1_4},

  {NOTE_F5,  DUR_1_8},
  {NOTE_DS5, DUR_1_8},
  {NOTE_D5,  DUR_1_8},
  {NOTE_C5,  DUR_1_8},

  {NOTE_AS4,  DUR_1_8},
  {NOTE_C5,  DUR_1_8},
  {NOTE_D5,  DUR_1_4},

  {NOTE_G4,  DUR_1_2},
};
const note_t melGolVag[] PROGMEM = {
  {NOTE_E4,  DUR_1_4},
  {NOTE_A4,  DUR_1_4},
  {NOTE_GS4, DUR_1_4},
  {NOTE_A4,  DUR_1_4},

  {NOTE_B4,  DUR_1_4},
  {NOTE_A4,  DUR_1_4},
  {NOTE_G4,  DUR_1_4},
  {NOTE_A4,  DUR_1_4},

  {NOTE_G4,  DUR_1_2},
  {NOTE_F4,  DUR_1_2},

  {NOTE_F4,  DUR_1_2P},
};
const note_t melHolms[] PROGMEM = {
  {NOTE_F4,  DUR_1_4PP},
  {NOTE_F4,  DUR_1_16},
  {NOTE_A4,  DUR_1_4},
  {NOTE_C5,  DUR_1_4},

  {NOTE_F5,  DUR_1_16},
  {NOTE_E5,  DUR_1_16},
  {NOTE_D5,  DUR_1_16},
  {NOTE_E5,  DUR_1_16},
  {NOTE_F5,  DUR_1_4P},
  {NOTE_E5,  DUR_1_4},
  {NOTE_D5,  DUR_1_8},

  {NOTE_C5,  DUR_1_16},
  {NOTE_AS4, DUR_1_16},
  {NOTE_A4,  DUR_1_16},
  {NOTE_AS4, DUR_1_16},
  {NOTE_C5,  DUR_1_8},
  {NOTE_C5,  DUR_1_16},
  {NOTE_C5,  DUR_1_16},
  {NOTE_C5,  DUR_1_8},
  {NOTE_AS4, DUR_1_4},
  {NOTE_A4,  DUR_1_8},

  {NOTE_A4,  DUR_1_2},
};
const note_t melKrOt[] PROGMEM = {
  {NOTE_B3,  DUR_1_4},
  {NOTE_E4,  DUR_1_4},
  {NOTE_G4,  DUR_1_4},

  {NOTE_FS4, DUR_1_4},
  {NOTE_E4,  DUR_1_4},
  {NOTE_G4,  DUR_1_4},
  {NOTE_E4,  DUR_1_4},

  {NOTE_FS4, DUR_1_4},
  {NOTE_E4,  DUR_1_4},
  {NOTE_C4,  DUR_1_4},
  {NOTE_D4,  DUR_1_4},

  {NOTE_B3,  DUR_1_2},
};
const note_t melNich[] PROGMEM = {
  {NOTE_E5,  DUR_1_4},
  {NOTE_D5,  DUR_1_4},
  {NOTE_C5,  DUR_1_4},

  {NOTE_C5,  DUR_1_4},
  {NOTE_B4,  DUR_1_4},
  {NOTE_A4,  DUR_1_4},

  {NOTE_B4,  DUR_1_4},
  {NOTE_B4,  DUR_1_4},
  {NOTE_B4,  DUR_1_4},

  {NOTE_B4,  DUR_1_4},
  {NOTE_GS4, DUR_1_4},
  {NOTE_E4,  DUR_1_4},
};

struct melody_t {
  unsigned int noteNumb;          // Count of notes
  unsigned int temp;
  const note_t *melody;
};

const melody_t melodies[] PROGMEM = {
  {sizeof(melImperMarch)/sizeof(note_t), 9, &melImperMarch[0]},
  {sizeof(melOyNaGori)/sizeof(note_t),   9, &melOyNaGori[0]},
  {sizeof(melProschSl)/sizeof(note_t),   7, &melProschSl[0]},
  {sizeof(melGolVag)/sizeof(note_t),     4, &melGolVag[0]},
  {sizeof(melHolms)/sizeof(note_t),      7, &melHolms[0]},
  {sizeof(melKrOt)/sizeof(note_t),       6, &melKrOt[0]},
  {sizeof(melNich)/sizeof(note_t),       6, &melNich[0]},
};

const unsigned char melodyCnt = sizeof(melodies)/sizeof(melody_t);

#endif
