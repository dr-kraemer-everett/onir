#include "dial.h"
#include "dial_device.h"

#include <Arduino.h>
#include <Wire.h>

Dial::Dial() {
  zero();
}
void Dial::attach(DialDevice* d) {
  device->init();
  device = d;
}

void Dial::update() {
  if (not switch_ready and not state.button) {
    switch_ready = true;
  }
  if (device) {
    device->read(state);
    return;
  }

  if (channel >= 0) {
    Wire.requestFrom(channel, (int)sizeof(DialState));
    
    if (Wire.available() == sizeof(DialState)) {
      Wire.readBytes((char*)&state, sizeof(DialState));
    }
    return;
  }

  // Not configured: no device, no channel
  if (millis() % 1000 == 0) {
    Serial.println("Dial not configured.");
  }
}

bool Dial::signal() {
    if (switch_ready and state.button) {
        switch_ready = false;
        return true;
    }
    return false;
}
