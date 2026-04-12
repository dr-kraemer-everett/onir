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
  Serial.println("Trimmer::execute");
  print_todo(todo);

  if (not (todo.command == Command::perform or todo.command == Command::modify)) {
    Serial.println("execute: no command");
    return release(todo);
  }
  Serial.print("execute: command: ");
  Serial.println((int)todo.command);

  if (not (todo.cue == Cue::drive)) {
    Serial.println("execute: not drive ");

    return release(todo);
  }
  Serial.println("execute: drive");

  if (not (todo.motion.motor == motion->motor)) {
    Serial.println("execute: wrong motor (logic error?)");

    return error(todo);
  }
  Serial.println("execute: correct motor");

  if (todo.direction == Cue::invert) {
    invert = true;
    Serial.println("execute: invert");

    return sign(todo);
  } else if (todo.direction == Cue::revert) {
    invert = false;
    Serial.println("execute: revert");
    return sign(todo);
  }

  if (read_dial()) {
    todo.motion.pitch = motion->pitch;
    Serial.print("execute: modify pitch: ");
    Serial.println(motion->pitch);
    return sign(todo, Command::modify);
  }
  Serial.println("execute: done");
  return mark(todo, Command::idle);
}

bool Trimmer::read_dial() {
  if (prior < reading) {
    (invert) ? pitch_up() : pitch_down();
    prior = reading;
    Serial.print("read_dial <: ");
    Serial.println(prior.count);
    return true;
  } else if (prior > reading) {
    (invert) ? pitch_down() : pitch_up();
    prior = reading;
    Serial.print("read_dial <: ");
    Serial.println(prior.count);
    return true;
  }
  return false;
}

bool Trimmer::pitch_down() {
  Serial.println("pitch_down: ");

  if (motion->pitch == (-1 << 7) + 1) {  // -127
    return false;
  }
  motion->pitch--;
  return true;
}

bool Trimmer::pitch_up() {
  Serial.println("pitch_up: ");
  if (motion->pitch == (1 << 7) - 1) {  // 127
    return false;
  }
  motion->pitch++;
  return true;
}
