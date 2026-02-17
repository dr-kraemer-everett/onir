#pragma once

#include "onir.h"

// defines an abstract display class. 
// no hardware-level details belong here.

#define BASE_CHANNEL 8  // address 8 is for your stuff. (https://i2cdevices.org/addresses)
#define DISPLAY_SHOW_ZERO true

// Client for seven-segment display device.
class CharDisplay {
public:
  bool is_device = false;  // write to screen over pinout
  bool is_client = false;  // write to device over wire

  CharDisplay(int channel = BASE_CHANNEL) {
    is_client = true;

    init(channel);
  }

  void init(int channel);

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
  
  void clear_str() {
    for (int i = 0; i < 4; i++) {
      put_char(i, ' ');
    }
    clear_point();
  }

  // call in loop()
  void refresh();

  // leave this alone.
  DisplayState state;

  static void test_fn();

protected:
  int channel = 8;
  const int UPDATE_FREQ_HZ = 50;
  const int UPDATE_MILLIS = 1000 / UPDATE_FREQ_HZ;
  long last_update = 0;

  void set_digits(int nnn) {
    for (int i = 0; i < 4; i++) {
      put_char(i, 0);  // 0 gives a blank screen
    }
    if ((nnn == 0) && !DISPLAY_SHOW_ZERO) return;
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

private:
  int absv(int n) {
    if (n > 0) return n;
    if (n == 0) return 0;
    return -n;
  }

  void send_update();
};
