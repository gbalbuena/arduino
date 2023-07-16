#include <math.h>

#include "MIDIUSB.h"
// Allows an Arduino board with USB capabilites to act as a MIDI instrument over USB.
// Micro, Pro Micro or Leonardo are required

#include "FREQUENCY_NOTES.h"
#include "MIDI_NOTES.h"

struct Button {
  int pin;
  int midiNote;
  
  bool state;
  bool previousState;

  long timer;
  long lastDebounceTime;
};

Button buttons[] = {
  { 30, C1, false, false, 0, 0 },
  { 31, D2, false, false, 0, 0 },
  { 32, E2, false, false, 0, 0 },
  { 33, F2, false, false, 0, 0 },
  { 34, G2, false, false, 0, 0 },
  { 35, A2, false, false, 0, 0 },
  { 36, B2, false, false, 0, 0 }
};

const int BUTTONS_LENGTH = sizeof(buttons) / sizeof(buttons[0]);
// const int BUZZER_PIN = 2;

void setup() {
  // Serial.begin(9600);
  MIDI.begin();

  for (int i = 0; i < BUTTONS_LENGTH; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
}

void loop() {
  handleButtons();
}

void handleButtons() {
    for (int i = 0; i < numButtons; i++) {
    buttons[i].state = digitalRead(buttons[i].pin);
    buttons[i].timer = millis() - buttons[i].lastDebounceTime;

    if (buttonsTimer[i] > buttonsTimeout) {
      lastDebounceTime[i] = millis();

      if (buttons[i].state != buttons[i].previousState) {

        // Button pressed, Send note-on message
        if (buttons[i].state == LOW) {
          // tone(BUZZER_PIN, midiToFreq(buttons[i].midiNote), 300);
          noteOn(0, buttons[i].midiNote, 64);
          MidiUSB.flush();

        // Button released, Send note-off message
        } else {
          // noTone(BUZZER_PIN);
          noteOff(0, buttons[i].midiNote, 0);
          MidiUSB.flush();
          
        }

        buttons[i].previousState = buttons[i].state;
        printInfo();
      }
    }
  }
}

int freqToMidi(float freq) {
  float midiNumber = 12 * (log(freq / 440.0) / log(2)) + 69;
  return round(midiNumber);
}

float midiToFreq(int midiNote) {
  float freq = pow(2, (midiNote - 69) / 12.0) * 440.0;
  return freq;
}

/**
  First parameter is the event type (0x09 = note on, 0x08 = note off).
  Second parameter is note-on/note-off, combined with the channel.
  Channel can be anything between 0-15. Typically reported to the user as 1-16.
  Third parameter is the note number (48 = middle C).
  Fourth parameter is the velocity (64 = normal, 127 = fastest).
 */
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void printInfo(int i) {
  Serial.print("[");
  Serial.print(i);
  Serial.print("] freq=");
  Serial.print(midiToFreq(buttons[i].midiNote));
  Serial.print(", midi=");
  Serial.println(buttons[i].midiNote);
}