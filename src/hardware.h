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

const int hardware_size = (int)PinFunction::END;  // TODO interface
using Hardware = int[hardware_size];
extern const Hardware no_hardware;

int assign(Hardware hardware, PinFunction fn, int pin);
int dispatch(const Hardware& hardware, PinFunction fn);
bool empty(const Hardware&);
