struct Timing {
  // id
  int channel = -1;
  
  // present
  long now_ms = -1;

  // past
  long seen_ms = -1;
  long gone_ms = -1;

  // policy
  int checkup_ms = 1000; // give it a second
  int update_ms = 10;
  int beats = 8;
  
};

static int call_beat(const Timing& timing) {
  if (timing.now_ms < 0) return -1;
  return timing.now_ms % timing.beats;
}


static bool go(const Timing& timing) {
  int beat = call_beat(timing);
  if (beat < 0) {
    // if you don't tell me what time it is, I won't tell you to go.
    return false;
  }
  
  return false;
}
