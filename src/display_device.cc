#include "display_device.h"

#include "Arduino.h"
#include "Wire.h"

DisplayDevice::DisplayDevice(const Hardware& hardware) : hardware(hardware) {
  for (int i = 0; i < 7; i++) {
    segments[i] = (PinFunction)(i + (int)PF::DD_A);
  }
  for (int i = 0; i < 4; i++) {
    positions[i] = (PinFunction)(i + (int)PF::DD_1);
  }
  set_segment_masks();
  set_char_masks();
  for (int i = (int)PinFunction::DD_A; i <= (int)PinFunction::DD_4; i++) {
    pinMode(dispatch(hardware, (PinFunction)i), OUTPUT);
  }
  clear();
}

void DisplayDevice::clear() {
  for (PinFunction segment : segments) {
    pin_low(segment);
  }
  set_point_pin(LOW);
  for (PinFunction position : positions) {
    pin_high(position);
  }
}

int DisplayDevice::position_to_show() {
  return (millis() % ms_per_cycle) / ms_per_digit;
}

void DisplayDevice::set_fn_pin(PinFunction fn, bool val) {
  digitalWrite(dispatch(hardware, fn), val);
}

void DisplayDevice::pin_high(PinFunction fn) {
  set_fn_pin(fn, HIGH);
}

void DisplayDevice::pin_low(PinFunction fn) {
  set_fn_pin(fn, LOW);
}

void DisplayDevice::set_point_pin(bool val) {
  set_fn_pin(PF::DD_P, val);
}

void DisplayDevice::refresh() {
  int position = position_to_show();
  if (position != position_showing) {  // redraw display
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
