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
    return pass(todo);
  }

  if (not (todo.cue == Cue::drive)) {
    return pass(todo);
  }

  if (not (todo.motion.motor == motion->motor)) {
    return pass(todo);
  }

  if (todo.direction == Cue::invert) {
    invert = true;
    return sign(todo);
  } else if (todo.direction == Cue::revert) {
    invert = false;
    return sign(todo);
  }

  if (read_dial()) {
    todo.motion.pitch = motion->pitch;
    return pass(todo, Command::modify);
   }
  return pass(todo);
}

bool Trimmer::read_dial() {
  if (reading.button) {
    motion->pitch = 0;
    return true;
  } else {
    if (prior < reading) {
      (invert) ? pitch_up() : pitch_down();
      prior = reading;
      return true;
    } else if (prior > reading) {
      (invert) ? pitch_down() : pitch_up();
      prior = reading;
      return true;
    }
  }
  return false;
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
