#include "program.h"

Action::Action(Cue cue)  {
  cue = cue;
}

Action::Action(const Instruction& instruction) : Instruction(instruction) { }
