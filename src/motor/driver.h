#pragma once

#include "motor/trimmer.h"
#include "motor/machine.h"


/// remove
/// remove
/// remove
#include "Arduino.h"
/// remove
/// remove
/// remove

class Driver {
public:

  Driver(Machine&);

  static Command drive(Program& program, Machine& machine);
  Command drive(Instruction& todo);

  Command drive();
  Command update();

private:

  static bool zero(Joint* joint, Program& program) {
    Serial.println("zero1");
    if (not joint) return false;
    Serial.println("zero2");
    if (joint->trimmer) return true;
    Serial.println("zero3");
    const Instruction& todo = program.instruction;
    if (todo.cue != Cue::drive) return false;
    Serial.println("zero4");
    Action*& drive = program[Cue::drive];
    if (not drive) {
      drive = new Action(todo);
      Serial.println("zero5");
    }
    Motion*& motion = (*drive)[todo.motion.motor];
    if (not motion) {
      motion = new Motion(todo.motion);
      Serial.println("zero6");
    }
    joint->trimmer = new Trimmer(todo.reading, motion);
    Serial.println("zero7");
    return true;
  }

  Program program;
  Machine& machine;
};
