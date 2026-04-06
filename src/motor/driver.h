#pragma once

#include "motor/trimmer.h"
#include "motor/machine.h"

class Driver {
public:

  Driver(Machine&);

  static Command execute(Program& program, Machine& machine);
  static Command drive(Program& program, Machine& machine);
  Command update();

private:

  bool trim(Joint* joint, Program& program) {
    if (not joint) return false;
    if (joint->ready()) return true;
    const Instruction& todo = program.instruction;
    if (todo.cue != Cue::drive) return false;
    Action*& drive = program[Cue::drive];
    if (not drive) {
      drive = new Action(todo);
    }
    Motion*& motion = (*drive)[todo.motion.motor];
    if (not motion) {
      motion = new Motion(todo.motion);
    }
    if (not joint->trimmer) {
      joint->trimmer = new Trimmer(todo.reading, motion);
    }
    return true;
  }

  Program program;
  Machine& machine;
};
