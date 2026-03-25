#pragma once

#include "motor.h"
#include "hardware.h"
#include "program.h"
#include "timing.h"

const int PULSE_NEUTRAL = 1500;
const int PULSE_HALF_SPAN = 500;

// converts motor pitch (signed 8 bit integer) into servo pulse in usec (16 bit)
int servo_pulse(s_small pitch);

class Servo;

class MotorDevice {
public:
  MotorDevice(const Hardware& hardware = no_hardware);

  void engage(Function function);
  void release(Function function);
  void set_pulse(Function function, int usec, long end_ms = 0);

  void update();
  void halt();

  int move();

private:

  Cue active = Cue::stop;

  Program program;

  template <typename T>
  static int execute(const Program& program, Resource<T>& resource);

  const Hardware& hardware;
  Resource<int> servo_pulses;
  Resource<long> end_millis;

  Resource<Servo*> robot;

  Rhythm rhythm;
};
