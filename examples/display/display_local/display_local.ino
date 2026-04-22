#include "circuits.h"
#include "display/display.h"

Hardware hardware = { };
Display* display { };

void banner() {
  display->put_str("onir");
  display->set_point(1);
  Serial.println("onir");
}

void setup() {
  Serial.begin(9600);
  uno_io(hardware);
  display = new Display(hardware);
  banner();
}

void loop() {
  display->refresh();
}
