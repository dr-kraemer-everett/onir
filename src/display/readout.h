#pragma once

#include "data.h"
#include "hardware.h"
#include "display.h"

class Readout {
public:
  Readout(Instruction*, Display*);
  Readout(Display*);
  Readout(Instruction* = 0, const Hardware& = no_hardware);
  void refresh();
  void report(Instruction*);

private:
  Display* display {};
  Instruction* instruction {};
  Motion prior {};
};
