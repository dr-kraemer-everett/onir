#include "dial.h"
#include "dial_device.h"

#include <Arduino.h>
#include <Wire.h>

Dial::Dial() { }

Dial::Dial(int ch) : Dial() {
  set_channel(ch);
}

Dial::Dial(Interface pinout) {
  attach(new DialDevice(pinout));
  zero();
}

void Dial::attach(DialDevice* d) {
  device = d;
  device->init();
}

void Dial::update() {
  if (not switch_ready and not state.button) {
    switch_ready = true;
  }
  if (device) {
    device->read(state);
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

long Dial::value() const {
  return state.count - zero_offset;
}
