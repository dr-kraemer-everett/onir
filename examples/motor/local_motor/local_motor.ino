#include "Wire.h"

#include "program.h"
#include "circuits.h"

#include "motor/driver.h"
#include "dial/dial.h"
#include "display/display.h"

Hardware hardware = {};
Machine* motor;
Dial* dial_l;
Dial* dial_r;
Display* display;
Motion run;
bool running = false;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  uno_car(hardware);
  motor = new Machine(hardware);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  dial_r->set_clock(Function::CLOCK_2);
  dial_r->set_data(Function::DATA_2);
  dial_r->set_switch(Function::SWITCH_2);
  display = new Display(9);
  motor->engage(Function::MOTOR_L_WHEEL, Target::rotation);
  motor->engage(Function::MOTOR_R_WHEEL, Target::rotation);
  Wire.begin();  // client
}

void loop() {
}
