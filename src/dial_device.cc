#include "dial_device.h"

#include <Arduino.h>

DialDevice::DialDevice(Interface p) : DialDevice() {
    set_pinout(p);
  }

void DialDevice::set_pinout(Interface p) {
  pinout = p;
  init();
}

void DialDevice::init() {
  pinMode(pinout[(int)PinFunction::CLOCK],  INPUT_PULLUP);
  pinMode(pinout[(int)PinFunction::DATA],   INPUT_PULLUP);
  pinMode(pinout[(int)PinFunction::SWITCH], INPUT_PULLUP);

  // assume the recent past was boring.
  clock = true;
}

bool DialDevice::clock_pin() const {
  return digitalRead(pinout[(int)PinFunction::CLOCK]);
}

bool DialDevice::data_pin() const {
  return digitalRead(pinout[(int)PinFunction::DATA]);
}

bool DialDevice::switch_pressed() const {
  return not digitalRead(pinout[(int)PinFunction::SWITCH]);
}

void DialDevice::read(DialState& state) {
  bool old_clock = clock;
  clock = clock_pin();
  if (clock != old_clock) {
    if (clock) {  // rising edge only
      if (data_pin()) {
        state.count--;
      } else {
        state.count++;
      }
    }
  }

  state.button = switch_pressed();
}
