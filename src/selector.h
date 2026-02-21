#include <Arduino.h>

#include "onir.h"

#include "dial.h"
#include "screen.h"

// uses a dial and screen to select a numerical I2C channel (int).
class Selector {
public:
  Selector(Dial* d, Screen* s) {
    dial = d;
    screen = s;
    channel = ONIR_CHANNEL;
  }

  Selector(int (*p)[(int)PinFunction::END]);

  int get_channel();


private:
  int channel;

  Dial* dial = nullptr;
  Screen* screen = nullptr;

  int (*pinout)[(int)PinFunction::END];
  
  
  const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
  const int max_channel = 0x77;        // 0x77 = 119
  // note: 192 - 8 = 184 = 8x23 => 32x23 char 'screen'


  char hex_digit(int value);

  void display_channel();


  void channel_up();
  void channel_down();

};

