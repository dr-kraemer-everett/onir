#include "display.h"
#include "Wire.h"

Display display(DISPLAY_DEVICE_CHANNEL);

void banner() {
  display.put_str("onir");
}

void flash() {
  display.set_point((millis() / 1000) % 2 ? -1 : 1);
}

void setup() {
  Serial.begin(9600);
  banner();
  Wire.begin();
  Serial.println("onir");
}

void loop() {
  flash();
  display.refresh();
}
