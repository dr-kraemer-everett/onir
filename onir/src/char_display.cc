#include "char_display.h"

#include "Arduino.h"
#include "Wire.h"

void CharDisplay::init(int channel_in) {
  state = { { -1, -1, -1, -1 },
            -1 };
  channel = channel_in;
}

void CharDisplay::refresh() {
  if ((long)millis() - last_update > UPDATE_MILLIS) {
    last_update = millis();
    send_update();
  }
}

String chars(DisplayState state) {
  char result[5];
  for (int i = 0; i < 4; i++) {
    result[i] = (char)state.chars[i];
  }
  return String(result);
}

void CharDisplay::send_update() {
  if (!is_client) return;  // only clients send updates.
  Wire.beginTransmission(channel);
  Wire.write((const byte*) &state, (int)sizeof(DisplayState));
  Wire.endTransmission();
}
