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

ServoControl control(Motion motion) {
  ServoControl control;
  control.pulse_usec = servo_pulse(motion.pitch);
  control.end_millis = end_millis(motion.duration);
  return control;
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    rhythm[fn].group = int(fn);
    rhythm[fn].layoff = 100;  // quick as a wink
  }
}

void MotorDevice::engage(Function function) {
  if (not robot[function]) {
    Servo* servo = new Servo;
    servo->attach(dispatch(hardware, function));
    robot[function].servo = servo;
    rhythm[function].last = UNSET;
    rhythm[function].missed = 0;
  }
}

void MotorDevice::release(Function function) {
  Servo* servo = robot[function].servo;
  if (servo) {
    robot[function].servo = 0;
    robot[function] = ServoControl();
    servo->detach();
    delete servo;
  }
}

void MotorDevice::assign(const Motion& motion) {
  if (robot[motion.motor]) {
    robot[motion.motor] = control(motion);
  }
}

// void MotorDevice::set_pulse(Function function, int usec, long end_ms) {
//   if (robot[function]) {
//     robot[function].pulse_usec = usec;

//     robot[function]->write(usec);
//     if (end_ms) {
//       robot[function].end_millis = end_ms;
//     }
//   }
// }

static Command MotorDevice::execute(Program& program, Resource<ServoControl>& robot) {
  Instruction todo = program.instruction;
  Command command = todo.command;
  switch (command) {
  case Command::none: { }
  case Command::perform: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          //assign(motion);
        }
      }
    }
  }

  case Command::extend: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          if (motion.motor == todo.motion.motor) {
            motion = todo.motion;
            return Command::extend;
          }
          if (true) {} //xxxx
      }
    }
  }

  }
  case Command::create: {
  }
  case Command::condition: {
  }
  }

  program.instruction.command = Command::none;  // clear command
  return command;
}

void MotorDevice::update() {
  //follow(rhythm, execute, program, robot);
}

int MotorDevice::move() {
//  for () {}
//  if () {}
  return 0;
}
