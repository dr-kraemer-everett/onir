#include "char_display.h"
#include "Wire.h"

CharDisplay display;

const unsigned int banner_time = 5000;

void banner() {
  display.put_char(0, 'o');
  display.put_char(1, 'n');
  display.put_char(2, 'i');
  display.put_char(3, 'r');
  display.set_point(1);
  while (millis() < banner_time) {
    display.refresh();
  }
  display.set_point(-1);
  for (int i = 0; i < 4; i++) {
    display.put_char(i, 0);
  }
  display.set_point(-1);
}

void setup() {
  Serial.begin(9600);
    Serial.println("starting test client.");
    Wire.begin();  // no address == host.
    banner();
}

long controller_step = -1;
const long millis_per_step = 1000;

void update_controller() {
  long step_now = max(0, (millis() - banner_time)) / millis_per_step;
  if (step_now > controller_step) {
    controller_step = step_now;
    Serial.print("step ");
    Serial.println(step_now);
    for (int i = 0; i < 4; i++) {
      display.put_char(i, (char)((int)'a' + (i + step_now) % 26));
    }
  }
}

void loop() {
  update_controller();
  display.refresh();
}
