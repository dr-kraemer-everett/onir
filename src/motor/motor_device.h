#pragma once

#include "motor.h"
#include "hardware.h"
#include "program.h"
#include "timing.h"

const int PULSE_NEUTRAL = 1500;
// const int PULSE_HALF_SPAN = 500;

// converts motor pitch (signed 8 bit integer) into servo pulse in usec (16 bit)
int servo_pulse(s_small pitch);

long end_millis(long duration);

struct ServoControl {
  int pulse_usec = UNSET;
  long end_millis = UNSET;
  Servo* servo = 0;

  operator bool() const {
    return servo;
  }

};

ServoControl control(Motion motion);

class Servo;

class MotorDevice {
public:
  MotorDevice(const Hardware& hardware = no_hardware);

  void engage(Function function);
  void release(Function function);
  void assign(const Motion& motion);
  // void set_pulse(Function function, int usec, long end_ms = 0);

  void update();  // call in loop()
  void halt();

  int move();

private:

  Cue active = Cue::stop;

  Program program;

  static Command execute(Program& program, Resource<ServoControl>& settings);

  const Hardware& hardware;
  Resource<ServoControl> robot = {};
  Resource<Rhythm> rhythm = {};
};
