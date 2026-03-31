#include "dial_device.h"

#include "Arduino.h"

DialDevice::DialDevice(const Hardware& hardware) : hardware(hardware) {
  if (not empty(hardware)) {
    pinMode(dispatch(hardware, Function::CLOCK),  INPUT_PULLUP);
    pinMode(dispatch(hardware, Function::DATA),   INPUT_PULLUP);
    pinMode(dispatch(hardware, Function::SWITCH), INPUT_PULLUP);
  }
  clock = true;   // assume the recent past was boring.
}

bool DialDevice::clock_pin() const {
  return digitalRead(dispatch(hardware, clock_fn));
}

bool DialDevice::data_pin() const {
  return digitalRead(dispatch(hardware, data_fn));
}

bool DialDevice::switch_pressed() const {
  return not digitalRead(dispatch(hardware, switch_fn));
}

void DialDevice::take(Reading& reading) {
  bool old_clock = clock;
  clock = clock_pin();
  if (clock != old_clock) {
    if (clock) {  // rising edge only
      if (data_pin()) {
        if (switch_pressed()) {
          reading.down_count--;
        } else {
          reading.count--;
        }
      } else {
        if (switch_pressed()) {
          reading.down_count++;
        } else {
          reading.count++;
        }
      }
    }
  }

  reading.button = switch_pressed();
}

void DialDevice::set_clock(Function fn) {
  clock_fn = fn;
}

void DialDevice::set_data(Function fn) {
  data_fn = fn;
}

void DialDevice::set_switch(Function fn) {
  switch_fn = fn;
}
