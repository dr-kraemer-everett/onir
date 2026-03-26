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

const int millis_per_wink = 100;  // a wink is half a blink.
long end_millis(u_small winks) {
  if (winks == 0) return UNSET;
  return millis() + winks * millis_per_wink;
}

Joint assign(Motion motion) {
  Joint joint;
  joint.pulse_usec = servo_pulse(motion.pitch);
  joint.end_millis = end_millis(motion.winks);
  return joint;
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    // rhythm[fn].group = int(fn);
    // rhythm[fn].layoff = 100;  // quick as a wink
  }
}

void MotorDevice::engage(Function function, Target target) {
  if (not robot[function]) {
    Joint& joint = robot[function];
    joint.servo = new Servo;
    joint.servo->attach(dispatch(hardware, function));
  }
}

void MotorDevice::release(Function function) {
  Servo* servo = robot[function].servo;
  if (servo) {
    robot[function].servo = 0;
    robot[function] = Joint();
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

static Command MotorDevice::execute(Program& program, Resource<Joint>& robot) {
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

static int MotorDevice::advance(Joint& joint) {
  if (not joint) return 0;

  const int delta = pulse_delta(joint);
  if (not delta) return 0;

  joint.pulse_usec += delta;
  joint.servo->write(joint.pulse_usec);
}

static bool stop_seek(Joint& joint) {
  if (not joint) return false;

  if (joint.target != Target::position) return false;

  joint.target_usec = joint.pulse_usec;  // try to stop here
  return true;
}

static bool stop_spin(Joint& joint) {
  if (not joint) return false;

  if (joint.target != Target::rotation) return false;

  joint.target_usec = PULSE_NEUTRAL;  // try to spin down
  return true;
}

static bool hold(Joint& joint) {
  if (not joint.servo) return false;

  if (joint.target == Target::position) return stop_seek(joint);

  if (joint.target == Target::rotation) return stop_spin(joint);
}
