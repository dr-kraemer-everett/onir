// CANNOT DO ANYTHING USEFUL
// work in progress

#include <Wire.h>

#include "io_device.h"
#include "uno_pinout.h"
#include "selector.h"

ScreenDevice device;
Screen screen;

// io_device.ino
// --------------
// Top-level transport wrapper for IODevice.
//
// IODevice knows nothing about I2C.
// This sketch binds Wire callbacks to IODevice::state.
//
// Extras for bench sanity:
//   - Channel selector at startup (local hardware shakedown)
//   - Periodic checksum print of IOState (every 3 seconds)

IODevice io;

int channel;

unsigned long last_report = 0;

// --- helpers ---

unsigned long checksum(const IOState& s) {
  const byte* p = (const byte*)&s;
  unsigned long sum = 0;
  for (int i = 0; i < (int)sizeof(IOState); i++)
    sum += p[i];
  return sum;
}

// --- I2C handlers ---

void on_receive(int n_bytes) {
  int n = n_bytes;
  if (n > (int)sizeof(ScreenState))
    n = (int)sizeof(ScreenState);

  Wire.readBytes((byte*)&io.state.screen, n);
}

void on_request() {
  Wire.write((byte*)&io.state.dial, (int)sizeof(DialState));
}

// --- Arduino lifecycle ---

void setup() {
 Serial.begin(9600);
  Serial.println("starting io display.");
  int* pinout = set_uno_pinout(init_interface);
  Dial dial;
  DialDevice dial_device;

  device.set_pinout(pinout);
  dial_device.set_pinout(pinout);
  dial.attach(&dial_device);
  screen.attach(&device);
  screen.set_point(-1);  // curious, this. must look into it.
  channel = Selector(&dial, &screen).get_channel();
  Serial.print("selected: ");
  Serial.println(channel);

  Wire.begin(channel);
  Wire.onReceive(on_receive);
  Wire.onRequest(on_request);
}

void loop() {
  io.update();

  unsigned long now = millis();
  if (now - last_report >= 3000) {
    last_report = now;
    io.state.screen.chars[3]++;
    Serial.print("state checksum: ");
    Serial.println(checksum(io.state));
  }
}