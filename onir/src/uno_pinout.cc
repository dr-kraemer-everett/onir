#include "uno_pinout.h"

int set_pin(int* pinout, PinFunction pf, int pin) {
  return pinout[(int)pf] = pin;
}

int* set_uno_pinout(int* pinout) {
  for (int i = 0; i < (int)PF::END; i++) {
    pinout[i] = 0;
  }

  // digit display
  set_pin(pinout, PF::DD_A, 6);
  set_pin(pinout, PF::DD_B, 2);
  set_pin(pinout, PF::DD_C, 10);
  set_pin(pinout, PF::DD_D, 12);
  set_pin(pinout, PF::DD_E, 13);
  set_pin(pinout, PF::DD_F, 5);
  set_pin(pinout, PF::DD_G, 9);
  set_pin(pinout, PF::DD_P, 11);
  set_pin(pinout, PF::DD_1, 7);
  set_pin(pinout, PF::DD_2, 4);
  set_pin(pinout, PF::DD_3, 3);
  set_pin(pinout, PF::DD_4, 8);

  // dial encoder
  
  set_pin(pinout, PF::CLOCK, 17);   // A3
  set_pin(pinout, PF::DATA, 16);    // A2
  set_pin(pinout, PF::SWITCH, 15);  // A1
  
  return pinout;
}
