#include "driver.h"

#include "Arduino.h"

bool logging = true;

void ll(String str) {
  if (logging) {
    Serial.println(str);
  }
}

Driver::Driver(Machine& machine) : machine(machine) {

  program[Cue::drive] = new Action(Cue::drive);

  Action* stop = new Action(Cue::stop);
  Action* go = new Action(Cue::go);

  program[Cue::stop] = stop;
  program[Cue::go] = go;

  for (Function motor = Function::MOTOR_MAIN; motor < Function::MOTOR_END; motor++) {
    Motion* halt =  new Motion();
    halt->motor = motor;
    (*stop)[motor] = halt;

    Motion* move =  new Motion();
    move->motor = motor;
    move->pitch = 1 << 5;
    (*go)[motor] = move;
  }
}

static Command Driver::drive(Program& program, Machine& machine) {
  ll("execute");
  Instruction& todo = program.instruction;
  if (not todo) {
    return sign(todo);                                      // nothing to report
  }

  const Motion& motion = todo.motion;
  Function function = motion.motor;
  if (motion and not is_motor(function)){
    return block(todo);                                    // nonsense
  }
  Joint* joint = machine[function];
  if (not joint) return fail(todo);                        // don't have one of those

  Command& command = todo.command;
  Cue cue = todo.cue;

  if (command == Command::perform or command == Command::modify) {
    ll("perform/modify");
    if (cue == Cue::none) {
      if (not motion) return block(todo);                 // nothing to perform/modify
      return mark(todo, machine.assign(motion));
    }
    if (cue == Cue::drive) {                                  // manual mode
      ll("drive");
      if (not motion) return block(todo);                     // can't drive nothing
      if (not joint->trimmer) zero(joint, program);           // set up trimmer
      Command response = joint->drive(todo);
      if (accept(response) and command == Command::perform) { // do it
	return mark(todo, machine.assign(todo.motion));       // pass to trimmer
      }
      return mark(todo, Command::modify);                     // updated drive motion
    }

    Action* action_ = program[cue];                           // preprogrammed action
    ll("cue");
    if (not action_) {
      if (not motion) {
	return fail(todo);                                    // don't know how to do nothing
      } else {
	action_ = new Action(todo);
      }
    }

    Action& action = *action_;
    Function motor = machine.assign(action);                  // queried or first found
    todo.motion = *action[motor];
    todo.direction = action.direction;
    todo.reading = action.reading;
    todo.message = action.message;
    return sign(todo);
  }

  if (command == Command::copy) {
    Cue direction = todo.direction;
    todo.direction = Cue::none;
    program[direction] = new Action(todo);
  }

  if (command == Command::condition) {
    return block(todo);
  }

  return block(todo);
}

Command Driver::drive(Instruction& todo) {
  program.instruction = todo;
  Command ret = drive();
  return mark(todo, ret);
}

Command Driver::drive() {
  Command response = drive(program, machine);
  machine.update();
  return mark(program.instruction, response);
}
