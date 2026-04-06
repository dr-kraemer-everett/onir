#pragma once
#include "timing.h"

// extends Instruction with multi-joint motions
class Action : public Instruction {
public:
  Action(Cue);
  Action(const Instruction&);
  operator bool() {
    return cue != Cue::none;
  }

  Motion* motions[(int)Function::COUNT] = { };
  Motion*& operator[](Function fn) {
    return motions[(int)cue];
  }
};

class Program {
public:

  Instruction instruction;  // client writes here by Wire.
  operator bool() {
    return instruction;
  }

  Action*& operator[](Cue cue) {
    return actions[(int)cue];
  }
  Action* actions[(int)Cue::count] = { };
};

template <typename T>
using Execute = int (*)(Program& program, Resource<T>& resource);

template <typename T>
static int follow(Rhythm& rhythm, Execute<T> execute, Program& program, Resource<T>& resource) {
  keep(rhythm);
  if (go(rhythm)) {
    rhythm.last = rhythm.now;

    int result = execute(program, resource);
    if (result) {
      rhythm.missed = 0;
    } else {
      rhythm.missed++;
    }

    return result;
  }

  return 0;
}
