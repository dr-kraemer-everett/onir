#pragma once

// names for logical pin functions.

enum class PinFunction {
  NONE,  // default

    // power pair
    GROUND,
    VCC,

    // dial
    CLOCK,
    DATA,
    SWITCH,

    // servo
    SERVO_L,
    SERVO_R,
    // SERVO_3,

    // seven segment digit display outputs

    // segments (anode)
    DD_A,
    DD_B,
    DD_C,
    DD_D,
    DD_E,
    DD_F,
    DD_G,
    DD_P,

    // positions (cathode)
    DD_1,
    DD_2,
    DD_3,
    DD_4,
    END,  // preserve trailing comma (dr. kraemer is old.)
  
    };

extern int init_interface[(int)PinFunction::END];

struct DisplayState {
  char chars[4];  // characters on screen; ' ' is blank.
  int point;     // using values outside [0,3] is pointless.
};

struct ServoState {
  int servo_l = 1500;
  int servo_r = 1500;
};
 
 
struct DialState {
  long count = 0;
  long down_count = 0;
  bool button = false;
};

struct IOState {
  DisplayState display;
  DialState dial;
  ServoState servo;
};
  
