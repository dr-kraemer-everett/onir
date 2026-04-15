#include "circuits.h"

#include "motor/driver.h"
#include "dial/dial.h"

#include "util.h"
#include "log.h"


Hardware hardware = {};
Machine* machine{};
Driver* driver{};
Operation* drive{};
Instruction drive_l{};
Instruction drive_r{};

Dial* dial_l{};
Dial* dial_r{};

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  machine = new Machine(hardware);
  driver = new Driver(*machine);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  dial_r->set_dial_2();

  machine->engage_hardware(Target::rotation);

  // driving instructions
  drive_l.command = Command::perform;
  drive_l.cue = Cue::drive;
  drive_l.reading.count = UNSET;
  drive_r = Instruction(drive_l);

  drive_l.motion.motor = Function::MOTOR_L_WHEEL;
  drive_r.motion.motor = Function::MOTOR_R_WHEEL;

  Serial.print("start loop (~");
  Serial.print(gamut<Instruction>());
  Serial.println("B free):");
}

void update_side(Dial* dial, Instruction& instruction) {
  Reading& reading = dial->update();
  if (reading != instruction.reading) {
    instruction.reading = reading;
    driver->drive(perform(instruction));
    print_instruction(instruction);
  }
}

void loop() {
  update_side(dial_l, drive_l);
  update_side(dial_r, drive_r);
  driver->drive();
}
