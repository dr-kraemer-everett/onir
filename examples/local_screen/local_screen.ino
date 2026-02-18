#include "screen.h"
#include "screen_device.h"
#include "uno_pinout.h"

Screen screen;
ScreenDevice device;

void banner() {
  screen.put_str("onir");
  screen.set_point(1);
  Serial.println("onir");
}

void setup() {
  Serial.begin(9600);
  device.set_pinout(set_uno_pinout(init_interface));
  screen.attach(&device);
  banner();
}

void loop() {
  screen.refresh();
}
