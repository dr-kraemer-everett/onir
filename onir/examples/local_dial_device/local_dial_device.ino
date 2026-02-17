#include "dial_encoder.h"
#include "uno_pinout.h"

DialEncoder dial;
int dial_value;

void setup() {
  Serial.begin(9600);
  dial.set_pinout(set_uno_pinout(init_interface));
  Serial.println("starting dial test");
  dial_value = dial.value();
}

void loop() {
  dial.update();
  if (dial_value != dial.value()) {
    Serial.println(dial.value());
    dial_value = dial.value();
  }
  if (dial.signal()) {
    Serial.println("zeroing (" + String(millis()) +")");
    dial.zero();
  }
}
