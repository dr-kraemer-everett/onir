#include "Wire.h"

#include "circuits.h"
#include "motor/control.h"
#include "log.h"

Hardware hardware = {};
Control* control = {};

const int channel = 0x09;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  uno_car(hardware);
  Wire.begin();  // client mode
  control = new Control(channel, hardware);
  memcheck();
}

void loop() {
  control->update();
}
