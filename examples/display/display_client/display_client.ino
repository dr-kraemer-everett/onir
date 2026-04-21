#include "Wire.h"

#include "channel.h"
#include "display/display.h"

Display display(number(Channel::display));

void flash() {
  display.set_point((millis() / 1000) % 2 ? -1 : 1);
}

void setup() {
  Serial.begin(9600);
  display.put_str("onir");
  Wire.begin();
  Serial.println("onir");
}

void loop() {
  flash();
  display.refresh();
}
