#include "trimmer.h"

#include "Arduino.h"


Trimmer::Trimmer(Machine* machine, Reading* reading, Dial* dial,
                 Function motor, s_small pitch,
                 Display* display, bool reversed) :
  machine(machine), reversed(reversed), display(display) {
  if (dial) {

  }
  run.motor = motor;
  run.pitch = pitch;
  run.winks = 0;  // no set end time
}

s_small Trimmer::pitch() {
  return run.pitch;
}

void Trimmer::update() {
  Reading last = dial->reading;
  dial->update();
  if (dial->press()) {
    run.pitch = 0;
    machine->assign(run);
  } else {
    if (last < dial->reading) {
      (reversed) ? pitch_up() : pitch_down();
    } else if (last > dial->reading) {
      (reversed) ? pitch_down() : pitch_up();
    }
  }
  machine->update();
}

bool Trimmer::pitch_down() {
  if (run.pitch == (-1 << 7) + 1) {  // -127
    Serial.println(run.pitch);
    return false;
  }
  run.pitch--;
  machine->assign(run);
  return true;
}

bool Trimmer::pitch_up() {
  if (run.pitch == (1 << 7) - 1) {  // 127
    Serial.println(run.pitch);
    return false;
  }
  run.pitch++;
  machine->assign(run);
  return true;
}
