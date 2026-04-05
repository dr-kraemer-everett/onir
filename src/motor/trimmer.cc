#include "trimmer.h"

#include "Arduino.h"


Trimmer::Trimmer(const Reading& reading, Motion* motion, bool invert = false) :
  reading(reading), prior(reading), motion(motion), invert(invert) { }

s_small Trimmer::pitch() {
  if (not motion) return 0;
  return motion->pitch;
}

Command Trimmer::execute(Instruction& todo) {
  if (not (todo.command == Command::perform or todo.command == Command::modify)) {
    return Command::none;  // no action taken, instruction unmodified.
  }

  if (not (todo.cue == Cue::drive)) {
    return Command::none;
  }

  if (not (todo.motion.motor == motion->motor)) {
    return Command::none;
  }
  if (todo.command == Command::perform) {
    read();

    // fill in current pitch and timeout for client to read
    todo.motion.pitch = motion->pitch;
    todo.motion.winks = motion->winks;  // TODO fix this
    return done(todo);
  } else if (todo.command == Command::modify) {
    if (todo.direction == Cue::invert) {
      invert = true;
      return done(todo);
    } else if (todo.direction == Cue::revert) {
      invert = false;
      return done(todo);
    }
    return Command::none;
  }
}

void Trimmer::read() {
  if (reading.button) {
    motion->pitch = 0;
  } else {
    if (prior < reading) {
      (invert) ? pitch_up() : pitch_down();
      prior = reading;
    } else if (prior > reading) {
      (invert) ? pitch_down() : pitch_up();
      prior = reading;
    }
  }
}

bool Trimmer::pitch_down() {
  if (motion->pitch == (-1 << 7) + 1) {  // -127
    return false;
  }
  motion->pitch--;
  return true;
}

bool Trimmer::pitch_up() {
  if (motion->pitch == (1 << 7) - 1) {  // 127
    return false;
  }
  motion->pitch++;
  return true;
}
