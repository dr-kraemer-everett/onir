#pragma once

#include "data.h"
#include "channel.h"

class Dial;
class Readout;
class Display;

struct Link {
  Dial* dial { };
  Instruction instruction { };

  operator bool() const {
    return dial;
  }

  Link() { };
  Link(const Hardware&);
  Link(Dial*);
  Link(Link*);
};

class Panel {
public:
  Panel() { }
  Panel(const Hardware&);

  Link*& operator[](Function fn) {
    return dials[(int)fn];
  }
protected:
  Link* dials[(int)Function::count] = { };
};

class Stem : public Panel {  // a pair of linked dials (ideally mounted co-axially)
public:

  Link*& left();
  Link*& right();

  operator bool() const {
    return left()->dial and right()->dial;
  }

  Stem() { };
  Stem(Dial* left, Dial* right);
  Stem(const Hardware& hardware);

private:
  Function fn_left =  Function::motor_l_wheel;
  Function fn_right = Function::motor_r_wheel;
};

class Control {
public:
  Control(Stem&, Display* = 0);
  Control(const Hardware&, Display* = 0);

  bool update();
  void address(Channel);
  void report(Instruction*);

private:
  Channel channel = Channel::car;
  void init();

  bool debrief_driver();

  bool update_link(Link*);

  static bool update_panel(const Instruction& incoming, Panel& panel);
  static void clear_device(Channel);
  static int read_instruction(Channel, Instruction* buffer);
  Panel panel { };
  Readout* readout { };
};
