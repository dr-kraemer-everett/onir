#pragma once

#include "onir.h"

// defines an abstract screen class. 
// no hardware-level details belong here.

#define SCREEN_DEVICE_CHANNEL 9  // 8 & 9 are just for your stuff.
#define MIN_CHANNEL 8            // (https://i2cdevices.org/addresses)
#define SCREEN_SHOW_ZERO true

class ScreenDevice;

// Client for seven-segment screen.
class Screen {
public:
  Screen() = default;
  
  Screen(int channel) {
    if (channel > MIN_CHANNEL) {
      init(channel);
    }
  }

  void init(int channel);

  void attach(ScreenDevice* d) {
    device = d;
  }
  
  void set_value(int value) {
    set_digits(value);
  }

  void set_point(int place) {
    state.point = place;
  }

  void clear_point() {
    state.point = -1;
  }

  void put_char(int place, char ch) {
    state.chars[place] = ch;
  }

  void put_str(const char* str) {  // beyond 4 dropped
    for (int i = 0; i < 4; i++) {
      put_char(i, str[i]);
    }
  }
  
  void clear() {
    for (int i = 0; i < 4; i++) {
      put_char(i, ' ');
    }
    clear_point();
  }

  // call in loop()
  void refresh();

  // leave this alone.
  ScreenState state;

private:
  ScreenDevice* device = nullptr;
  
  int channel = -1;  // don't call if nothing will answer. (the modem rings SYN for a minute.)

  const int UPDATE_FREQ_HZ = 50;
  const int UPDATE_MILLIS = 1000 / UPDATE_FREQ_HZ;
  long last_update = -1;

  void set_digits(int nnn) {
    for (int i = 0; i < 4; i++) {
      put_char(i, 0);  // 0 gives a blank screen
    }
    if ((nnn == 0) && !SCREEN_SHOW_ZERO) return;
    int sign = nnn / absv(nnn);
    if (sign < 0) {
      put_char(0, (int)('-'));
    }
    int value = absv(nnn);
    // assign value's lowest three place values to the display's digits.
    // higher place values are discarded.
    for (int k = 3; k >= 1; k--) {
      int modulus = 1;  // C++, y u no (int) pow?
      for (int j = 0; j < 4 - k - 1; j++) {
        modulus *= 10;
      }
      put_char(k, (value / modulus) % 10);
      if (!(value / (modulus * 10))) return;
    }
  }

  int absv(int n) {
    if (n > 0) return n;
    if (n == 0) return 0;
    return -n;
  }
  
  void send_update();
  void update_local();
 
};

