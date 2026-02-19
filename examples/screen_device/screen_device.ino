#include "screen_device.h"
#include "uno_pinout.h"

#include "Wire.h"

ScreenDevice screen;

void update_screen(int message_size) {
  Wire.readBytes((byte*)&screen.state, message_size);
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting screen.");
  screen.set_pinout(set_uno_pinout(init_interface));

  Wire.begin(SCREEN_DEVICE_CHANNEL);
  Wire.onReceive(update_screen);
}

void loop() {
  screen.refresh();
}
