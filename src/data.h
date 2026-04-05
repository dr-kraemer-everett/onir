#pragma once

#include "hardware.h"

struct Reading {
  int count = 0;
  int down_count = 0;
  bool button = false;

  bool operator==(const Reading& other) const {
    return (count == other.count and
            down_count == other.down_count and
            button == other.button);

  };

  bool operator!=(const Reading& other) const {
    return not operator==(other);

  };

  bool operator<(const Reading& other) const {
    if (count != other.count) {
      return count < other.count;
    }
    if (down_count != other.down_count) {
      return down_count < other.down_count;
    }
    return false;
  }

  bool operator>(const Reading& other) const {
    if (count != other.count) {
      return count > other.count;
    }
    if (down_count != other.down_count) {
      return down_count > other.down_count;
    }
    return false;
  }
};

struct Message {
  char chars[4] = {0, 0, 0, 0} ;  // characters on display
  s_small point = UNSET;          // values outside [0,3] are pointless.

  void clear() {
    for (int i = 0; i < 4; i++) {
      chars[i] = ' ';
    }
  }

};

struct Motion {
  Function motor = Function::NONE;
  operator bool() const {
    return motor != Function::NONE;
  }

  s_small pitch = 0;
  u_small winks = 10;  // try for a second

  void clear() {
    motor = Function::NONE;
    pitch = 0;
    winks = 0;
  }

};

enum class Cue : u_small {
  none,   // unset
  stop,   // static response for motor type ("stay there" or "don't spin")
  go,     // default action

  // act
  forward,
  back,
  spin_clockwise,
  spin_counterwise,
  go_right,
  go_left,
  back_right,
  back_left,

  // follow
  drive,   // set pitch by dial
  scan,    // watch IR sensor

  // modify
  invert,  // flip dial meaning
  revert,  // normal reading

  count,  // last item used for size
};

enum class Command : u_small {
  none, //
  perform,    // showtime
  ignore,     // driver indicates bad instruction

  modify,     // add or modify motion for cue
  copy,       // duplicate cue (uses .direction)
  forget,     // delete object

  condition,  // NOT IMPLEMENTED (place condition on cue)

};

struct Instruction {
  s_small channel = UNSET;

  Command command = Command::none;    // boss to driver
  Command respond = Command::none;    // driver to boss
  operator bool() const {
    return command != Command::none and respond != Command::ignore;
  }

  Cue cue = Cue::go;                // engage motor cue
  Motion motion;
  Cue direction = Cue::stop;        // next action / command argument

  Message message;                  // displays
  Reading reading;                  // sensors

};

static Command apply(Command respond, Instruction& todo) {
  todo.respond = respond;
  return respond;
}

static Command done(Instruction& todo) {
  Command command = todo.command;
  todo.command = Command::none;
  return apply(command, todo);
}

static Command ignore(Instruction& todo) {
  return apply(Command::ignore, todo);
}

static Command reject(Instruction& todo) {
  todo.command = Command::none;
  return ignore(todo);
}
