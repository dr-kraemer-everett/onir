#pragma once

#include "hardware.h"
#include "dial/dial.h"
#include "display/display.h"

#include "machine.h"



class Trimmer {
public:
  Trimmer(const Reading&, Motion* motion, bool invert = false);

  s_small pitch();

  bool execute(Instruction& todo);

  void update();

private:

  const Reading& reading;
  Reading prior;  // copy needed to spot changes

  Motion* motion;

  bool invert = false;

  bool pitch_down();
  bool pitch_up();
};
