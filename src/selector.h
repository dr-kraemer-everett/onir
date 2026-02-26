#pragma once

#include "onir.h"

#include "dial.h"
#include "display.h"

// uses a dial and display to select a numerical I2C channel (int).
class Selector {
public:
  Selector(Dial* dial, Display* display, bool button=false, const Hardware& hardware = no_hardware);

  Selector(const Hardware& hardware = no_hardware); 

  void set_button(bool button) { button_down = button; }

  int get_channel();

private:
  int channel;
  void channel_up();
  void channel_down();
  const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
  const int max_channel = 0x77;        // 0x77 = 119
  // note: 192 - 8 = 184 = 8x23 => 32x23 char 'display'

  Dial* dial = nullptr;
  Display* display = nullptr;

  bool button_down;
  bool done();
  int count();

  char hex_digit(int value);
  void display_channel();
  void echo();
  
  const Hardware& hardware;
};

