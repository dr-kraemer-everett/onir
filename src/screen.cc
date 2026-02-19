#include "screen.h"

#include "Arduino.h"
#include "Wire.h"

#include "screen_device.h"

void Screen::init(int c) {
  channel = c;
  Wire.begin();
}

void Screen::refresh() {
  if ((long)millis() - last_update > UPDATE_MILLIS) {
    last_update = millis();
    send_update();
    update_local();
  }
  if (device) {
    device->refresh();
  }
}

String chars(ScreenState state) {
  char result[5];
  for (int i = 0; i < 4; i++) {
    result[i] = (char)state.chars[i];
  }
  return String(result);
}

void Screen::send_update() {
  if (channel < 0) return;
  Wire.beginTransmission(channel);
  Wire.write((const byte*) &state, (int)sizeof(ScreenState));
  Wire.endTransmission();
}


void Screen::update_local() {
  if (device){
  device->update(state);  
  }
}
