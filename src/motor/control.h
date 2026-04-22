#pragma once

#include "data.h"
#include "channel.h"
#include "dial/panel.h"

class Dial;
class Readout;
class Display;

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
