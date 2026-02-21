#pragma once

#include "onir.h"

class DialDevice;

class Dial {
public:
  Dial();
  Dial(int ch);
  Dial(Interface pinout);

  void attach(DialDevice* d);
  
  void set_channel(int ch) {
    channel = ch;
  }

  void update();
  bool signal();

  long value() const;
  
  bool button() const {
    return state.button;
  }

  void zero() {
    zero_offset = state.count;
  }

  DialState state;

private:
  int channel = -1;  // -1 means unset
  long zero_offset = 0;
  
  bool switch_ready = false;

  DialDevice* device = nullptr;
};
