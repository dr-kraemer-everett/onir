#include "driver.h"

static Command Driver::execute(Program& program, Machine& machine) {
  Instruction& todo = program.instruction;
  Command& command = todo.command;
  switch (command) {

  case Command::none: {
    return done(command);
  }

  case Command::create: {
    // see if cue already exists
    for (const Action& action : program.actions) {
      if (action.cue == todo.cue) {
        return nope(command);
      }
    }
    static_assert(PROGRAM_SIZE > (int)Cue::count);
    // find a free spot
    for (Action& action : program.actions) {
      if (not action) {
        action.cue = todo.cue;
        if (todo.motion) {
          machine.assign(todo.motion);
        }
        return done(command);
      }
    }

    // no room left
    return nope(command);
  }

  case Command::perform: {
    bool trigger = false;
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          machine.assign(motion);
          trigger = true;
        }
      }
    }
    if (trigger) {
      return done(command);
    } else {
      return nope(command);
    }
  }

  case Command::modify: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          if (motion.motor == todo.motion.motor) {
            motion = todo.motion;
            return done(command);
          }
        }
      }
    }
    return nope(command);
  }

  case Command::condition: {
    return nope(command);
  }

  return nope(command);
  }
}

Command Driver::update() {
  return execute(program, machine);
  machine.update();
}
