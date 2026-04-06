#include "driver.h"

Driver::Driver(Machine& machine) : machine(machine) {

  // set up stop, go, drive actions

// set up trimmers
}

static Command Driver::execute(Program& program, Machine& machine) {
  Instruction& todo = program.instruction;
  if (not todo) {
    return done(todo);
  }
  Command& command = todo.command;
  Cue cue = todo.cue;
  const Motion& motion = todo.motion;

  if (command == Command::perform) {
    if (cue == Cue::none) {
      if (not motion) return reject(todo);  // need cue or motion to perform
      return machine.assign(motion) ? done(todo) : ignore(todo);
    }
    Action* action_ = program[cue];
    if (not action_) {
      return ignore(todo);
    }
    Action& action = *action_;
    Function response = machine.assign(action);
    if (response == Function::NONE) {
      return ignore(todo);
    }
    Motion& motion = *action[response];
    todo.motion = motion;
    todo.direction = action.direction;
    return done(todo);
  }

  if (command == Command::modify) {
  // see if cue exists
    Action*& action = program[cue];
    if (not action) {
      action = new Action(todo);
    }
    Motion& motion = action->motion;
    if (motion) {
      action->motion = motion;
      if (action->cue == Cue::drive) {
        //            if (not panel[action->cue]) {
        //   panel[action->cue] = new Trimmer(&this, &action->reading, motion.motor);
        // }
      }

    }
    action->direction = todo.direction;
    action->message = todo.message;
    action->reading = todo.reading;
    return done(todo);
  }

  if (command == Command::condition) {
    return reject(todo);
  }

  return ignore(todo);
}


Command Driver::update() {
  return execute(program, machine);
  machine.update();
}
