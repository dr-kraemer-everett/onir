#include "Arduino.h"
#include "Servo.h"

#include "motor_device.h"

// TODO: add custom per-servo trim (data file?)
int servo_pulse(s_small pitch) {
  // // bit shift by 7 ==> divide by 2^7.
  // long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  // return PULSE_NEUTRAL + (magnitude >> 7);

  return PULSE_NEUTRAL + pitch * 4;  // (this is fine 4 now.)
}

long end_millis(long duration) {
  if (duration == UNSET) return UNSET;
  return millis() + duration;
}

ServoParams perform(Motion motion) {
  ServoParams params;
  params.pulse_usec = servo_pulse(motion.pitch);
  params.end_millis = end_millis(motion.duration);
  return params;
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    Rhythm& rhythm = rhythms[fn];
    rhythm.group = int(fn);
    rhythm.layoff = 100;  // quick as a wink
  }
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
    settings[function].pulse_usec = usec;
    robot[function]->write(usec);
    if (end_ms) {
      settings[function].end_millis = end_ms;
    }
  }
}

static int MotorDevice::execute(Program& program, Resource<ServoParams>& settings) {
  switch (program.instruction.command) {
  case Command::none:
    return 0;
  case Command::perform:
    for (const Action& action : program.actions) {
      if (action) {
      }
    }
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
  //follow(rhythm, execute, program, settings);
}

int MotorDevice::move() {
//  for () {}
//  if () {}
  return 0;
}
