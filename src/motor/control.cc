#include "Arduino.h"
#include "Wire.h"

#include "control.h"
#include "log.h"

#define CONTROL_LOG true

Link::Link(Dial* dial) : dial(dial) { }

Link::Link(const Hardware& hardware) : dial(new Dial(hardware)) { }

Link::Link(Link* link ) {
  dial = link->dial;
  instruction = Instruction {link->instruction};
}

Panel::Panel(const Hardware& hardware) {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    if (is_set(hardware, fn)) {
      Serial.println((int)fn);
      Link*& link = operator[](fn);
      link = new Link(hardware);
      link->instruction.cue = Cue::drive;
      link->instruction.motion.motor = fn;
      print_instruction(link->instruction);
    }
  }
}

Stem::Stem(const Hardware& hardware) {
  left()  = new Link(new Dial(hardware));
  right() = new Link(new Dial(hardware));
  right()->dial->set_dial_2();
}

Link*& Stem::left() {
  return dials[(int)fn_left];
}

Link*& Stem::right() {
  return dials[(int)fn_right];
}

Stem::Stem(Dial* l, Dial* r) {
  left() = new Link(l);
  right() = new Link(r);
  r->set_dial_2();
}

void Control::init() {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    Link* link = panel[fn];
    if (link) {
      if (fn == Function::MOTOR_R_WHEEL) {
        Serial.println("Dial 2");
        link->dial->set_dial_2();
      }
      Instruction& todo = link->instruction;
      todo.channel = channel;
      todo.command = Command::modify;
      todo.cue = Cue::drive;
    }
  }
}

Control::Control(int channel, Stem& stem) :  channel(channel) {
  init();
}

Control::Control(int channel, const Hardware& hardware) :
  panel(hardware), channel(channel) {
  init();
}

static void Control::clear_device(int channel) {
  const static Instruction blank { };
  Wire.beginTransmission(channel);
  Wire.write((char*)&blank, sizeof(Instruction));
  Wire.endTransmission(channel);
}

// num bytes if response; false for missing data or bad response.
static int Control::read_instruction(int channel, Instruction* buffer) {
  if (not buffer) return 0;

  Wire.requestFrom(channel, sizeof(Instruction));
  if (Wire.available() == sizeof(Loop)) {
    Wire.readBytes((char*)buffer, sizeof(Loop));  // prefix read
    return sizeof(Instruction);

  } else if (Wire.available() == sizeof(Instruction)) {
    Wire.readBytes((char*)buffer, sizeof(Instruction));
    if (CONTROL_LOG) {
      Serial.print("incoming:  ");
      print_instruction(*buffer);
    }
    return sizeof(Instruction);

  } else if (Wire.available() == 0) {
    Serial.print("no driver. m: ");
    Serial.println(millis());
    return 0;

  } else {
    Serial.print("bad driver! m: ");
    Serial.println(millis());
    return 0;
  }
}

static bool Control::update_panel(const Instruction& incoming, Panel& panel) {
  Link* link = panel[incoming.motion.motor];
  if (not link) return false;
  link->instruction = incoming;
  return true;
}

// true if good response; false for missing or failed response.
bool Control::debrief_driver() {
  static Instruction todo { };
  bool done = false;
  while (not done) {
    int response_size = read_instruction(channel, &todo);
    if (response_size == sizeof(Instruction)) {  // got it.
      done = true;
    }

    if (response_size == sizeof(Loop)) {
      if (empty(todo)) {
        done = true;      // nothing there.

      } else if (todo) {  // still working
        delay(2);         // this should be quick -- try again.
      } else {
        Serial.println("???");
        return false;
      }
    }
  }

  const bool saved = update_panel(todo, panel);
  clear_device(channel);
  return saved and completed(todo);
}

bool Control::update_link(Link* link) {

  if (not link) return false;
  link->dial->update();
  Instruction& todo = link->instruction;
  if (todo.reading != link->dial->reading) {
    if (not debrief_driver()) {
      Serial.println("driver error?");
    }
    todo.reading = link->dial->reading;
    if (todo.command == Command::none) {
      todo.command = Command::perform;
    }
    todo.respond = Command::none;
    Wire.beginTransmission(channel);
    Wire.write((char*)&todo, sizeof(Instruction));
    Wire.endTransmission(channel);
    update_panel(todo, panel);
    if (CONTROL_LOG) {
      Serial.print("outgoing: ");
      print_instruction(todo);
    }
  }
}

Command Control::update() {
//  Serial.println(millis());
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    update_link(panel[fn]);
  }
}
