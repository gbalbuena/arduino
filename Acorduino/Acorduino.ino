// #include "MIDIUSB.h"
// Allows an Arduino board with USB capabilites to act as a MIDI instrument over USB.
// Micro, Pro Micro or Leonardo are required

#include "FREQUENCY.h"
#include "MIDI_NOTES.h"

const int BUTTONS_QTY = 14;
const int BUZZER_PIN = 2;
const byte BUTTONS_PINS[BUTTONS_QTY] = { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 22, 24, 26 };

byte buttonsState[BUTTONS_QTY] = { 0 };
byte buttonsPreviousState[BUTTONS_QTY] = { 0 };

unsigned long lastDebounceTime[BUTTONS_QTY] = { 0 };
unsigned long buttonsTimer[BUTTONS_QTY] = { 0 };
int buttonsTimeout = 10; // ms (10-30)

// Diatonic
int note1[BUTTONS_QTY] = {
  C1, D2, E2, F2, G2, A2, B2,
  C3, D3, E3, F3, G3, A3, B3
};

int note2[BUTTONS_QTY] = {
  C4, D4, E4, F4, G4, A4, B4,
  C5, D5, E5, F5, G5, A5, B5
};

int note3[BUTTONS_QTY] = {
  C6, D6, E6, F6, G6, A6, B6,
  C7, D7, E7, F7, G7, A7, B7
};

boolean isPlaying = false;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < BUTTONS_QTY; i++) {
    pinMode(BUTTONS_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  _handleButtons();
}

void _handleButtons() {
  for (int i = 0; i < BUTTONS_QTY; i++) {
    buttonsState[i] = digitalRead(BUTTONS_PINS[i]);
    buttonsTimer[i] = milis() - lastDebounceTime[i];

    if (buttonsTimer[i] > buttonsTimeout) {
      lastDebounceTime[i] = milis();

      if (buttonsState[i] == LOW) {
        isPlaying = true;
        tone(BUZZER_PIN, note1[i], 300);
      } else {
        isPlaying = false;
        noTone(BUZZER_PIN);
      }

      if (buttonsState[i] != buttonsPreviousState[i]) {
        buttonsPreviousState[i] = buttonsState[i];

        Serial.print("[");
        Serial.print(i);
        Serial.print("]");
        Serial.println(buttonsState[i]);
      }
    }
  }
}
