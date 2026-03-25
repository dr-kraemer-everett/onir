#include "Servo.h"

#include "motor_device.h"

// TODO: add custom per-servo trim (data file?)
int servo_pulse(s_small pitch) {
  // // bit shift by 7 ==> divide by 2^7.
  // long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  // return PULSE_NEUTRAL + (magnitude >> 7);

  return PULSE_NEUTRAL + pitch * 4;  // (this is fine 4 now.)
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) {
  rhythm.layoff = 100;  // quick as a wink
}

void MotorDevice::engage(Function function) {
  if (not robot[function]) {
    Servo* servo = new Servo;
    servo->attach(dispatch(hardware, function));
    robot[function] = servo;
  }
  set_pulse(function, PULSE_NEUTRAL);
}

void MotorDevice::release(Function function) {
  Servo* servo = robot[function];
  if (servo) {
    robot[function] = 0;
    set_pulse(function, 0);
    servo->detach();
    delete servo;
  }
}

void MotorDevice::set_pulse(Function function, int usec, long end_ms) {
  if (robot[function]) {
    servo_pulses[function] = usec;
    robot[function]->write(usec);
    if (end_ms) {
      end_millis[function] = end_ms;
    }
  }
}

template <typename T>
static int MotorDevice::execute(const Program& program, Resource<T>& resource) {
  switch (program.instruction.command) {
  case Command::none:
    return 0;
  case Command::perform:
    return 1;
  case Command::extend:
    return 0;
  case Command::create:
    return 0;
  case Command::condition:
    return 0;
  default:
    return 0;
  }
}

void MotorDevice::update() {
  follow(rhythm, execute, program, robot);
}

int MotorDevice::move() {
//  for () {}
//  if () {}
  return 0;
}
