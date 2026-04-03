#pragma once

#include "hardware.h"
#include "dial/dial.h"
#include "display/display.h"

#include "machine.h"



class Trimmer {
public:
  Trimmer(Machine* , Reading* = 0, Dial* = 0,
          Function = Function::MOTOR_MAIN, s_small pitch = 0,
          Display* = 0, bool reversed = false);

  s_small pitch();

  void update();

private:

  Reading* reading;
  Reading local;  // copy for detecting changes
  Dial* dial = 0;
  Display* display = 0;

  Machine* machine = 0;

  Motion run { };

  bool reversed = false;
  bool pitch_down();
  bool pitch_up();
};
