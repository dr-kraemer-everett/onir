#pragma once

#include "hardware.h"

struct Message {
  char chars[4] = {0, 0, 0, 0} ;  // characters on display
  s_small point = UNSET;             // values outside [0,3] are pointless.
};

struct Motion {
  Function motor = Function::NONE;
  s_small pitch = 0;
  long duration = 1000;  // try for a second

  void clear() {
    motor = Function::NONE;
    pitch = 0;
    duration = 0;
  }

  operator bool() const {
    return motor != Function::NONE;
  }
};

enum class Cue {
  stop,

  go, // default action to modify

  // actions
  forward,
  back,
  spin_clockwise,
  spin_counterwise,
  go_right,
  go_left,
  back_right,
  back_left,

  // follows
  drive,   // set pitch by dial
  scan,    // watch IR sensor

  count,  // last item used for size
};
