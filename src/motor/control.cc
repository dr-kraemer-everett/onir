#include "Arduino.h"
#include "Wire.h"

#include "control.h"
#include "log.h"

Link::Link(Dial* dial) : dial(dial) { }

Link::Link(const Hardware& hardware) : dial(new Dial(hardware)) { }

Stem::Stem(const Hardware& hardware) {
  left  = new Dial(hardware);
  right = new Dial(hardware);
  right->set_dial_2();
}

void Control::init() {
  left.instruction.channel = channel;
  left.instruction.command = Command::modify;
  left.instruction.cue = Cue::drive;
  right.instruction = left.instruction;
  Serial.print("start right:   ");
  print_instruction(right.instruction);

  Serial.print("start left:    ");
  print_instruction(left.instruction);
  right.instruction.motion.motor = Function::MOTOR_R_WHEEL;
  left .instruction.motion.motor = Function::MOTOR_L_WHEEL;
  right.dial->set_dial_2();
  write_link(left);
  delay(20);   // TODO: loop read, wait for device to mark done.
  write_link(right);
}

Control::Control(int channel, Stem& stem) :
  left(stem.left), right(stem.right), channel(channel) {
  init();
}

Control::Control(int channel, const Hardware& hardware) :
  left(hardware), right(hardware), channel(channel) {
  init();
}

void Control::print_new(const Instruction& instruction, Instruction& prior) {
  if (prior != instruction) {
    prior = instruction;
    print_instruction(instruction);
  }
}

static const void Control::read_link(Link& link) {
  Serial.print("Control::read_link: ");
  const int channel = link.instruction.channel;
  const int count = sizeof(Instruction);
  char* buffer = (char*)&link.instruction;
  Wire.requestFrom(channel, count);

  if (Wire.available() == count) {
    Wire.readBytes(buffer, count);
    Serial.print("incoming: ");
    print_instruction(link.instruction);
  } else {
    Serial.println("(x) ");
  }
}

static const void Control::write_link(Link& link) {
  Serial.print("Control::write_link: ");
  const int channel = link.instruction.channel;
  const int count = sizeof(Instruction);
  char* buffer = (char*)&link.instruction;
  Wire.beginTransmission(channel);
  Wire.write(buffer, count);
  Wire.endTransmission(channel);
  Serial.print("outgoing: ");
  print_instruction(link.instruction);
}

static const Instruction& Control::update_link(Link& link) {
  if (link.dial->update() != link.instruction.reading) {
    read_link(link);
    link.instruction.reading = link.dial->reading;
    link.instruction.command = Command::perform;
    link.instruction.respond = Command::none;
    write_link(link);
    return link.instruction;
  }
}

Command Control::update() {
  update_link(left);
  update_link(right);
}
