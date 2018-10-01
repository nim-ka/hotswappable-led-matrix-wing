#include "Adafruit_LEDBackpack.h"
#include <Adafruit_SleepyDog.h>

#define LED_PIN 13
#define MATRIX_ADDRESS 0x70

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();

void setup () {
  Wire.begin();
  
  pinMode(LED_PIN, OUTPUT);
  
  Watchdog.enable(50);
  
  do {
    Wire.beginTransmission(MATRIX_ADDRESS);

    Wire.write(0x21);
    Wire.write(0x80 | 0x01 | (0 << 1));
    Wire.write(0xE0 | 15);
    
    digitalWrite(LED_PIN, HIGH);
  } while (Wire.endTransmission() != 0);
  
  matrix.begin(MATRIX_ADDRESS);

  Watchdog.disable();

  digitalWrite(LED_PIN, LOW);
}

// LED Matrix code goes here! Make sure to call `writeDisplay` instead of `matrix.writeDisplay`
int state = LED_OFF;

void loop () {
  for (int i = -1; i <= 8; i ++) {
    matrix.drawRect(-1, -1, 8 - i, 15 - i * 2,
      state == LED_OFF ? LED_ON : LED_OFF);
    matrix.drawRect(-1, -1, 8 - i, 16 - i * 2,
      state == LED_OFF ? LED_ON : LED_OFF);
    
    writeDisplay();

    delay(50);
  }

  state = state == LED_OFF ? LED_ON : LED_OFF;
}

void writeDisplay () {
  int ms = Watchdog.enable(50);
  
  Wire.beginTransmission(MATRIX_ADDRESS);
  
  Wire.write(0x00);
  
  for (int i = 0; i < 8; i ++) {
    Wire.write(matrix.displaybuffer[i] & 0xFF);    
    Wire.write(matrix.displaybuffer[i] >> 8);    
  }
  
  if (Wire.endTransmission() != 0) {
    delay(ms + 100);
  }

  Watchdog.disable();
}
