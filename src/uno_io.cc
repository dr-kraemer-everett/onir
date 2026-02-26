#include "uno_io.h"

void uno_io(Hardware hardware) {

  for (int fn = (int)PF::NONE + 1; fn < (int)PF::END - 1; fn++) {
    assign(hardware, (PinFunction)fn, -1); // it's rude to yell at the modem
  }

  // digit display
  assign(hardware, PF::DD_A, 6);
  assign(hardware, PF::DD_B, 2);
  assign(hardware, PF::DD_C, 10);
  assign(hardware, PF::DD_D, 12);
  assign(hardware, PF::DD_E, 13);
  assign(hardware, PF::DD_F, 5);
  assign(hardware, PF::DD_G, 9);
  assign(hardware, PF::DD_P, 11);
  assign(hardware, PF::DD_1, 7);
  assign(hardware, PF::DD_2, 4);
  assign(hardware, PF::DD_3, 3);
  assign(hardware, PF::DD_4, 8);

  // dial encoder
  
  assign(hardware, PF::CLOCK, 17);   // A3
  assign(hardware, PF::DATA, 16);    // A2
  assign(hardware, PF::SWITCH, 15);  // A1
}

