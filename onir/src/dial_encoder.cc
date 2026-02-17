

#include "dial_encoder.h"

#include "Arduino.h"

bool DialEncoder::clock_pin() {
  return digitalRead(pinout[(int)PinFunction::CLOCK]);
}
bool DialEncoder::data_pin() {
  return digitalRead(pinout[(int)PinFunction::DATA]);
}
bool DialEncoder::switch_pressed() {
  return not digitalRead(pinout[(int)PinFunction::SWITCH]);
}

int DialEncoder::get_pin(PinFunction fn) {
  return pinout[(int)fn];
}

void DialEncoder::set_pinout(int* pinout_in) {
  for (PinFunction fn = PinFunction::CLOCK; fn <= PinFunction::SWITCH;
       fn = (PinFunction)((int)fn + 1)) {
    pinout[(int)fn] = pinout_in[(int)fn];
    if (fn == PinFunction::SWITCH) {
      pinMode(get_pin(fn), INPUT);;
    } else {
      pinMode(get_pin(fn), INPUT_PULLUP);  // unconnected dial will not flicker changes.
    }
  }
  clock_state = clock_pin();
}

// check for dial movement.
void DialEncoder::update() {
  bool old_clock_state = clock_state;
  clock_state = clock_pin();
  if (clock_state != old_clock_state) {
    if (clock_state) {
      if (data_pin()) {
        movement--;
      } else {
        movement++;
      }
    }
  }
  if (not switch_ready and not switch_pressed()) {
    switch_ready = true;
  }
}

bool DialEncoder::signal() {
  if (switch_ready and switch_pressed()) {
    switch_ready = false;
    return true;
  } else {
    return false;
  }
}
