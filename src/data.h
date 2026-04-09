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

  query,   // ask about motor motion

  count,   // last item used for size
};

inline bool is_motor(Cue cue) {
  int i = (int)cue;
  return (i >= (int)Cue::stop and i <=(int)Cue::scan);
}

enum class Command : u_small {
  none, //
  perform,    // showtime
  ignore,     // driver indicates bad instruction

  modify,     // add or modify motion for cue
  copy,       // duplicate cue (uses .direction)
  forget,     // delete object

  condition,  // NOT IMPLEMENTED (place condition on cue)

};

static bool accept(const Command& command) {
  if (command == Command::none) return false;
  if (command == Command::ignore) return false;
  return true;
}

struct Instruction {
  s_small channel = UNSET;

  Command command = Command::none;    // boss to driver
  Command respond = Command::none;    // driver to boss
  operator bool() const {
    return command != Command::none and respond != Command::ignore;
  }

  Cue cue = Cue::go;          // engage motor cue
  Motion motion;

  Message message;            // displays
  Reading reading;            // sensors

  Instruction* next = 0;      // next intruction; if none, try direction
  Cue direction = Cue::none;

  void clear() {
    Instruction blank;
    *this = blank;
  }
};

static Command pass(Instruction& todo, Command response) {
  return response;
}

static Command pass(Instruction& todo) {
  return pass(todo, Command::none);
}

static Command mark(Instruction& todo, Command response) {
  todo.respond = response;
  pass(todo, response);
}

static Command apply(Command response, Instruction& todo) {
  todo.respond = response;
  pass(todo, response);
}

static Command sign(Instruction& todo) {
  Command command = todo.command;
  todo.command = Command::none;
  return mark(todo, command);
}

static bool done(Instruction& todo) {
  return todo.respond == Command::none and accept(todo.respond);
}

static bool modified(Instruction& todo) {
  return todo.command != Command::none and todo.respond != Command::none;
}

static Command fail(Instruction& todo) {
  Command command = todo.command;
  todo.command = Command::none;
  return mark(todo, Command::ignore);
}

static bool failed(Instruction& todo) {
  return todo.respond == Command::ignore;
}

static Command block(Instruction& todo) {
  todo.command = Command::ignore;
  return fail(todo);
}

static bool blocked(Instruction& todo) {
  return todo.command == Command::ignore;
}

static bool modified(Instruction& todo, Command response) {
  return response == Command::modify or todo.respond == Command::modify;
}
