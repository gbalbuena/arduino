// File: CascadeSwitch16Inputs-74HC165.ino
//
// Version 1, 5 August 2021, by Koepel
//     Initial version.
// Version 2, 5 August 2021, by Koepel
//     Layout of the wiring made better.
// Version 3, 13 August 2021, by Koepel
//     Changed 'SCK' to 'clockPin'.
// Version 4, 6 August 2023 by Gabriel Balbuena
//     Changed to use push buttons and 16 inputs
//
// Cascade of two 74HC165 shift-in registers.
// Only three pins are used on the Arduino board, to read 16 switches.
//
// Using the 74HC165 is safe, because a pulse to the Latch pin
// ('PL' on the 74HC165) will make a new start every time.
// In case of an error or a wrong clock pulse by noise,
// it synchronizes the data when inputs are read the next time.
//
// Based on:
//   (1)
//     Demo sketch to read from a 74HC165 input shift register
//     by Nick Gammon, https://www.gammon.com.au/forum/?id=11979
//   (2)
//     74HC165 Shift register input example
//     by Uri Shaked, https://wokwi.com/arduino/projects/306031380875182657
//

const byte latchPin = 9;      // to latch the inputs into the registers
const byte clockPin = 13;     // I choose the SCK pin
const byte dataPin = 12;      // I choose the MISO pin
uint16_t oldOptionSwitch = 0; // previous state of all the inputs

const int pulseWidth = 10;    // pulse width in microseconds

void setup() {
  Serial.begin(115200);
  Serial.println("Top row is switch 0 (right) to switch 7 (left)");
  Serial.println("Second row is 8 to 15, and so on");

  pinMode(clockPin, OUTPUT); // clock signal, idle LOW
  pinMode(latchPin, OUTPUT); // latch (copy input into registers), idle HIGH
  digitalWrite(latchPin, HIGH);
}

void loop() {
  // Give a pulse to the parallel load latch of all 74HC165
  digitalWrite(latchPin, LOW);
  delayMicroseconds(pulseWidth);
  digitalWrite(latchPin, HIGH);

  // Reading one 74HC165 at a time and combining them into a 16 bit variable
  uint16_t optionSwitch = 0;
  for (int i = 8; i >= 0; i -= 8) {
    optionSwitch |= ((uint16_t)ReadOne165()) << i;
  }

  for (int i = 0; i < 16; i++) {
    if (bitRead(optionSwitch, i) != bitRead(oldOptionSwitch, i)) {
      Serial.print("Switch ");
      if (i < 10) {
        Serial.print(" ");
      }
      Serial.print(i);
      Serial.print(" is now ");
      Serial.println(bitRead(optionSwitch, i) == 0 ? "down ↓" : "up   ↑");
    }
  }

  oldOptionSwitch = optionSwitch;
  delay(25); // slow down the sketch to avoid switch bounce
}

// The ReadOne165() function reads only 8 bits,
// because of the similar functions shiftIn() and SPI.transfer()
// which both use 8 bits.
//
// The shiftIn() can not be used here, because the clock is set idle low
// and the shiftIn() makes the clock high to read a bit.
// The 74HC165 require to read the bit first and then give a clock pulse.
byte ReadOne165() {
  byte ret = 0x00;

  // The first one that is read is the highest bit (input D7 of the 74HC165).
  for (int i = 7; i >= 0; i--) {
    if (digitalRead(dataPin) == HIGH) {
      bitSet(ret, i); 
    }
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(clockPin, LOW);
  }

  return (ret);
}
