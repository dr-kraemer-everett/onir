#include "screen_device.h"

#include "Arduino.h"
#include "Wire.h"

ScreenDevice::ScreenDevice() {
  for (int i = 0; i < 7; i++) {
    segments[i] = (PinFunction)(i + (int)PF::DD_A);
  }
  for (int i = 0; i < 4; i++) {
    positions[i] = (PinFunction)(i + (int)PF::DD_1);
  }
  for (int i = 0; i < (int)PinFunction::END; i++) {
    device_pinout[i] = 0;
  }
}

void ScreenDevice::set_pinout(int* pinout) {
  set_segment_masks();
  set_char_masks();
  for (int i = (int)PinFunction::DD_A; i <= (int)PinFunction::DD_4; i++) {
    device_pinout[i] = pinout[i];
    pinMode(pinout[i], OUTPUT);
  }
  clear_pins();
}

void ScreenDevice::clear_pins() {
  for (PinFunction output : segments) {
    digitalWrite(device_pinout[(int)output], LOW);
  }
  digitalWrite(device_pinout[(int)PF::DD_P], LOW);
  for (PinFunction output : positions) {
    digitalWrite(device_pinout[(int)output], HIGH);
  }
}

void ScreenDevice::debug() {
  for (int i = 0; i < 4; i++) {
    Serial.print((char)state.chars[i]);
  }
  Serial.print(digitalRead(A3));
  Serial.println();
}

void ScreenDevice::debug(char c) {
  Serial.print(c);
  Serial.print(": ");
  debug();
}

void ScreenDevice::debug(int i) {
  Serial.print(i);
  Serial.print(": ");
  Serial.println(device_pinout[i]);
}

int ScreenDevice::position_to_show() {
  return (millis() % ms_per_cycle) / ms_per_digit;
}

void ScreenDevice::refresh() {
  int position = position_to_show();
  if (position != position_showing) {  // redraw screen
    position_showing = position;
    clear_pins();
    for (PinFunction segment : segments) {
      if (segment_masks[(int)segment] & char_masks[(int)state.chars[position]]) {
        digitalWrite(device_pinout[(int)segment], HIGH);
      }
    }
    if (state.point == position) {
      digitalWrite(device_pinout[(int)PF::DD_P], HIGH);
    }
    digitalWrite(device_pinout[(int)positions[position]], LOW);
  }
}
