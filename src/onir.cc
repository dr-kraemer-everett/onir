#include "onir.h"
#include "control.h"

#include "Arduino.h"

Interface init_interface;

int interface_size = (int)PinFunction::END;

Onir::Onir(int* channels, int count) {
  control = new Control(channels, count);
}


Onir::update() {
  control->update();
}
