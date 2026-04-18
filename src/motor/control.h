#pragma once

#include "data.h"
#include "operation.h"
#include "dial/dial.h"

struct Stem {  // a pair of dials (ideally mounted co-axially)
  Dial* left { };
  Dial* right { };

  operator bool() const {
    return left and right;
  }

  Stem() { };
  Stem(Dial* left, Dial* right) : left(left), right(right) { };
  Stem(const Hardware& hardware);
};

struct Link {
  Dial* dial { };
  Instruction instruction { };

  operator bool() const {
    return dial;
  }

  Link() { };
  Link(const Hardware&);
  Link(Dial*);

};

class Control {
public:
  Control(int channel, Stem&);
  Control(int channel, const Hardware&);

  Command update();

private:
  void init();
  void Control::print_new(const Instruction&, Instruction& prior);

  static const void read_link(Link&);
  static const void write_link(Link&);
  static const Instruction& update_link(Link&);
  Instruction left_prior { };
  Instruction right_prior { };

  const int channel;

  Link left;
  Link right;
};
