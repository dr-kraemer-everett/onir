#include "Wire.h"

#include "channel.h"
#include "circuits.h"
#include "motor/control.h"
#include "log.h"

Hardware hardware = {};
Control* control = {};

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  Wire.begin();  // client mode
  control = new Control(number(Channel::car), hardware);
  memcheck();
}

void loop() {
  control->update();
}
