#include "trimmer.h"

#include "Arduino.h"


Trimmer::Trimmer(const Reading& reading, Motion* motion, bool invert = false) :
  reading(reading), prior(reading), motion(motion), invert(invert) { }

s_small Trimmer::pitch() {
  if (not motion) return 0;
  return motion->pitch;
}

void Trimmer::update() {
  if (reading.button) {
    motion->pitch = 0;
  } else {
    if (prior < reading) {
      (invert) ? pitch_up() : pitch_down();
      prior = reading;
    } else if (prior > reading) {
      (invert) ? pitch_down() : pitch_up();
      prior = reading;
    }
  }
}

bool Trimmer::pitch_down() {
  if (motion->pitch == (-1 << 7) + 1) {  // -127
    return false;
  }
  motion->pitch--;
  return true;
}

bool Trimmer::pitch_up() {
  if (motion->pitch == (1 << 7) - 1) {  // 127
    return false;
  }
  motion->pitch++;
  return true;
}

bool Trimmer::execute(Instruction& instruction) {
  return false;
}
