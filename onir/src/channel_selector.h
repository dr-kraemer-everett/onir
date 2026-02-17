#include <Arduino.h>

#include "onir.h"

#include "dial_encoder.h"
#include "char_device.h"

// uses a dial and screen to select a numerical I2C channel (int).
class ChannelSelector {
public:
  ChannelSelector(DialEncoder* dial_in, CharDevice* screen_in) {
    dial = dial_in;
    screen = screen_in;
    channel = min_channel;
  }

  int get_channel();


private:
  int channel;

  DialEncoder* dial;
  CharDevice* screen;
  
  const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
  const int max_channel = 0x77;        // 0x77 = 119
  // note: 192 - 8 = 184 = 8x23 => 32x23 char 'screen'


  char hex_digit(int value);

  void display_channel();


  void channel_up();
  void channel_down();


};

