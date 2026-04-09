#include "program.h"

Action::Action(Cue cue) {
  cue = cue;
  extend();
}

Action::Action(const Instruction& instruction) : Instruction(instruction) { }

Command Action::extend(const Instruction& instruction) {
  if (instruction.direction == Cue::query) return;
  const Motion& motion = instruction.motion;
  if (not instruction or not motion) return Command::ignore;
  Motion*& location = operator[](motion.motor);
  if (not location) {
    location = new Motion;
  }
  *location = motion;
}

Command Action::extend() {
  extend(*this);
}
