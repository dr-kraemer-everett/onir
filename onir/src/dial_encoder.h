#pragma once

#include "onir.h"

// TODO:
// DialEncoder doesn't obey the interface rules from char_display:
// - represent canonical state using the corresponding state struct from onir.h.

// - offer two classes:
// -- client:
// --- owns state and interface
// --- no knowledge of pins or dependence on uno io pinout
// --- must EITHER:
// ---- (1) also be a local device (below)
// ---- (2) have a channel -> pull state from Wire on update()
// -- device:
// --- knows about io pinout -> will read pins on update()

// also:

// - signal state (signal_ready) is in client
// -- (switch == one bit-> entropy says you can't do better than echo it directly)

// - need to track down_count in update()
// -- just add a simple branch on switch_pressed() at both changes to movement.
// -- new client (local_dial_down_count.ino)
// --- prints down_count to modem when pressed
// --- also still zeros.
// -- and add a zero_down_count() method
// --- add zero_down_count.ino:
// ---- same, but zeros down_count, not count
// ----- also, zeros target when _released_, not on signal(). (duplicate logic locally)
// ----- (yes, this is just because the symmetry amuses me.)

// - my desktop demos must still work. (obvs.)
// - add sketches:
// -- dial_client.ino (reads state over Wire, no weird code needed)
// -- dial_device.ino
// --- hard-coded channel = 0x08
// --- interrupt handler that sends state to client in main .ino file.
// - don't make any sweeping changes that aren't needed. (this should be 3/4 copy&paste.)
// - match my style.
// - if you see a mistake, please point it out. (preferably with a citation)

class DialEncoder {
public:
 
  void update();  // call in loop()  

  void set_pinout(int* pinout);  // call in setup()
 
  void zero() {  // code can call this on signal()
    zero_offset = movement;
  }

  long value() {
    return movement - zero_offset;
  }

  // returns true once / press of the switch.
  bool signal();

private:
  bool clock_pin();
  bool data_pin();
  bool switch_pressed();

  int get_pin(PinFunction pin);

  long zero_offset = 0;
  
  int pinout[(int)PinFunction::END];

  bool clock_state = false;

  long movement = 0;

  bool switch_ready = false;
};
