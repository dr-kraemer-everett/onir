#include "trimmer.h"
#include "log.h"

#include "Arduino.h"

Trimmer::Trimmer(const Reading& reading, Motion* motion, bool invert = false) :
  reading(reading), prior(reading), motion(motion), invert(invert) { }

s_small Trimmer::pitch() {
  if (not motion) return 0;
  return motion->pitch;
}

Command Trimmer::execute(Instruction& todo) {
  if (not performative(todo.command)) return error(todo);

  if (not (todo.cue == Cue::drive)) return error(todo);

  if (not (todo.motion.motor == motion->motor)) return error(todo);

  if (todo.direction == Cue::invert) {
    invert = true;
    Serial.println("execute: invert");

    return sign(todo, Command::modify);
  } else if (todo.direction == Cue::revert) {
    invert = false;
    Serial.println("execute: revert");
    return sign(todo, Command::modify);
  }

  if (read_dial()) {
    todo.motion.pitch = motion->pitch;
    return mark(todo, Command::modify);
  }
  return mark(todo, Command::idle);
}

bool Trimmer::read_dial() {
  if (prior < reading) {
    (invert) ? pitch_up() : pitch_down();
    prior = reading;
    return true;
  } else if (prior > reading) {
    (invert) ? pitch_down() : pitch_up();
    prior = reading;
    return true;
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
