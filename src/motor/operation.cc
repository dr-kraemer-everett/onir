#include "operation.h"

Operation::Operation(Cue cue) {
  cue = cue;
  extend();
}

Operation::Operation(const Instruction& instruction) : Instruction(instruction) { }

Code Operation::extend(const Instruction& instruction) {
  if (instruction.direction == Cue::query) return Code::idle;
  const Motion& motion = instruction.motion;
  if (not instruction or not motion) return Code::idle;
  Motion*& location = operator[](motion.motor);
  if (not location) {
    location = new Motion;
  }
  *location = motion;
  return Code::modify;
}

Code Operation::extend() {
  return extend(*this);
}
