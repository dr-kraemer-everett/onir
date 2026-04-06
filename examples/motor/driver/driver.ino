#include "circuits.h"
#include "motor/driver.h"
#include "util.h"

Driver* driver;
Machine* machine;
Hardware hardware = {};

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  uno_car(hardware);
  machine = new Machine(hardware);
  driver = new Driver(*machine);
  Serial.print("gamut: ");
  Serial.println(gamut<Instruction>());
}

void loop() {
  delay(1000);
}
  