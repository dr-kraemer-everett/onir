#include "char_display.h"
#include "Wire.h"

CharDisplay display;

void banner() {
  display.put_str("onir");
  display.set_point(1);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();  // no address == host.
  banner();
  Serial.println("onir");
}

void loop() {
  display.refresh();
}
