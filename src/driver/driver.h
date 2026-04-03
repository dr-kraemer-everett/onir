#pragma once

#include "driver_device.h"

#include "program.h"
#include "motor/machine.h"

class Driver {

  static Command execute(Program& program, Machine& machine);
  Command update();

  Program program;
  Machine& machine;
};
