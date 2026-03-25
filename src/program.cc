#include "program.h"

Action::Action() { }

void Action::forget() {
  for (Motion& motion : motions) {
    motion.clear();
  }
  n_motions = 0;
}

void Program::clear(const Cue cue) {
  for (Action& action: actions) {
    if (action.cue == cue) {
      action.forget();
    }
  }
}
