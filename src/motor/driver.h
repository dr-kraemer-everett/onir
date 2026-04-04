#pragma once

#include "motor/trimmer.h"

class Driver {
public:

  Driver(Machine&);

  static Command execute(Program& program, Machine& machine);
  Command update();

private:

  Program program;
  Machine& machine;
};
