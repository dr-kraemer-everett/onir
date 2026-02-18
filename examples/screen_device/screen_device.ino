//#include "char_device.h"
#include "screen_device.h"
#include "uno_pinout.h"

#include "Wire.h"

//CharDevice device;
ScreenDevice screen;

void update_display_state(int message_size) {
  Wire.readBytes((byte*)&screen.state, message_size);
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting screen.");
  screen.set_pinout(set_uno_pinout(init_interface));

  Wire.begin(SCREEN_DEVICE_CHANNEL);
  Wire.onReceive(update_display_state);
}

void loop() {
  screen.refresh();
}
