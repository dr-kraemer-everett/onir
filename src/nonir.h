#pragma once

#include "hardware.h"

class Control;
class Screen;

class Control {
public:
  Control(int* ch, int ct) {
    channels = ch;
    count = ct;
  }
  int* channels;
  int count;
};

class Screen {
public:
  Screen(Control* ctl) {
    control = ctl;
  }
  Control* control;
};  

class Nonir {
public:
  Nonir(int* channels, int count, const Hardware& hw = no_hardware);
//  Nonir(int* channels, int count);
  Control* control;
  Screen* screen;
  const Hardware& hardware;
};

