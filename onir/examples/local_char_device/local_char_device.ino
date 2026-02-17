#include "char_device.h"
#include "uno_pinout.h"

CharDevice device;

void banner() {
  device.put_str("onir");
  device.set_point(1);
  Serial.println("onir");
}

void setup() {
  Serial.begin(9600);
  device.set_pinout(set_uno_pinout(init_interface));
  banner();
}

void loop() {
  device.refresh();
}
