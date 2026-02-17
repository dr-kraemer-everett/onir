#include "char_device.h"
#include "uno_pinout.h"

#include "Wire.h"

CharDevice device;

void update_display_state(int message_size) {
  if (!message_size) {
    return;
  }
  byte* p = (byte*)&device.state;
  for (unsigned int i = 0; i < (unsigned int)message_size; i++) {
    *(p + i) = Wire.read();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting character display.");
  Wire.begin(BASE_CHANNEL);
  device.set_pinout(set_uno_pinout(init_interface));
  Wire.onReceive(update_display_state);
}

void loop() {
  device.refresh();
}
