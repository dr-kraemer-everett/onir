#pragma once

#include "data.h"

// extends Instruction with multi-joint motions
class Operation : public Instruction {
public:
  Operation(Cue);
  Operation(const Instruction&);
  operator bool() {
    return cue != Cue::none;
  }

  Code extend(const Instruction& todo);   // update from todo.motion
  Code extend();                          // update from Action::motion

  Motion* motions[(int)Function::count] = { };
  Motion*& operator[](Function fn) {
    return motions[(int)fn];
  }
};
