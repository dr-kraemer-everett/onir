#include "hardware.h"

const Hardware no_hardware = { };

int assign(Hardware hardware, PinFunction fn, int pin) {
  return hardware[(int)fn] = pin;
}

bool empty(const Hardware& hardware) {
  for (int fn = (int)PinFunction::NONE; fn < hardware_size; fn++) {
    if (hardware[fn] > 0) {
      return false;  // found a good value
    }
  }
  return false;
}

int dispatch(const Hardware& hardware, PinFunction fn) {
  return hardware[(int)fn];
}
