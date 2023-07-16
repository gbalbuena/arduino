// #include "MIDIUSB.h"
#include "pitches.h"

const int BUTTONS_QTY = 14;

byte BUTTONS_PINS[BUTTONS_QTY] = { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43 };
byte BUTTONS_STATE[BUTTONS_QTY] = { 0 };
byte BUTTONS_PREVIOUS_STATE[BUTTONS_QTY] = { 0 };

const int BUZZERL_PIN = 2;

const int OCTAVE = 6 * 12;

// Diatonic
// int note[BUTTONS_QTY] = {
//   0,    2,    4,    5,    7,    9,    11,
//   0+12, 2+12, 4+12, 5+12, 7+12, 9+12, 11+12
// };

// Chromatic
// int note[BUTTONS_QTY] = {
//   0, 1, 2,  3,  4,  5,  6,
//   7, 8, 9, 10, 11, 12, 13
// };

int note[BUTTONS_QTY] = {
  1,2,3,4,5,6,7,8,9,10,11, 1,2,3
};

boolean isPlaying = false;

void setup() {
  Serial.begin(9600);
  // Serial.begin(115200);

  for (int i = 0; i < BUTTONS_QTY; i++) {
    pinMode(BUTTONS_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  _handleButtons();
}

void _handleButtons() {
  for (int i = 0; i < BUTTONS_QTY; i++) {
    BUTTONS_STATE[i] = digitalRead(BUTTONS_PINS[i]);

    if (BUTTONS_STATE[i] == LOW) {
      isPlaying = true;
    } else if (
      BUTTONS_STATE[0] && BUTTONS_STATE[1] && BUTTONS_STATE[2] && BUTTONS_STATE[3] && BUTTONS_STATE[4] && BUTTONS_STATE[5] && BUTTONS_STATE[6] && BUTTONS_STATE[7] && BUTTONS_STATE[8] && BUTTONS_STATE[9] && BUTTONS_STATE[10] == HIGH) {
      isPlaying = false;
    }

    for (int i = 0; i < BUTTONS_QTY; i++) {
      if (BUTTONS_STATE[i] == LOW) {
        float freq = midiToFreq(note[i] + OCTAVE);
        Serial.println(freq);
        tone(BUZZERL_PIN, 440);
      }
      if (isPlaying == false) {
        noTone(BUZZERL_PIN);
      }
    }

    if (BUTTONS_STATE[i] != BUTTONS_PREVIOUS_STATE[i]) {
      BUTTONS_PREVIOUS_STATE[i] = BUTTONS_STATE[i];

      Serial.print(i);
      Serial.print('> ');
      Serial.println(BUTTONS_STATE[i]);
    }
  }
}

float midiToFreq(float midival) {
  float f = 0.0;
  if (midival) f = 8.1757989156 * pow(2.0, midival / 12.0);
  return f;
}
