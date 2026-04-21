#pragma once

#include "data.h"

class Dial;

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
  Control(int channel, Stem&);
  Control(int channel, const Hardware&);

  bool update();

private:
  const int channel;
  void init();

  bool debrief_driver();

  bool update_link(Link*);

  static bool update_panel(const Instruction& incoming, Panel& panel);
  static void clear_device(int channel);
  static int read_instruction(int channel, Instruction* buffer);
  Panel panel { };
};
