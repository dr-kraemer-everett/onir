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
    device_pinout[i] = -1;
  }
  set_segment_masks();
  set_char_masks();
}

void ScreenDevice::set_pinout(int* pinout) {
  for (int i = (int)PinFunction::DD_A; i <= (int)PinFunction::DD_4; i++) {
    device_pinout[i] = pinout[i];
    pinMode(pinout[i], OUTPUT);
  }
  clear();
}

void ScreenDevice::clear() {
  for (PinFunction segment : segments) {
    pin_low(segment);
  }
  set_point_pin(LOW);
  for (PinFunction position : positions) {
    pin_high(position);
  }
}

int ScreenDevice::position_to_show() {
  return (millis() % ms_per_cycle) / ms_per_digit;
}

void ScreenDevice::pin_high(PinFunction fn) {
  set_fn_pin(fn, HIGH);
}

void ScreenDevice::pin_low(PinFunction fn) {
  set_fn_pin(fn, LOW);
}

void ScreenDevice::set_fn_pin(PinFunction fn, bool val) {
  digitalWrite(device_pinout[(int)fn], val);
}

void ScreenDevice::set_point_pin(bool val) {
  digitalWrite(device_pinout[(int)PF::DD_P], val);
}

void ScreenDevice::refresh() {
  int position = position_to_show();
  if (position != position_showing) {  // redraw screen
    position_showing = position;
    clear();
    for (PinFunction segment : segments) {
      if (segment_masks[(int)segment] & char_masks[(int)state.chars[position]]) {
        pin_high(segment);
      }
    }
    if (state.point == position) {
      set_point_pin(HIGH);
    }
    pin_low(positions[position]);
  }
}
