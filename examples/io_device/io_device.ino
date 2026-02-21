// Binds Wire callbacks to addresses in IODevice::state.

#include <Wire.h>

#include "io_device.h"
#include "uno_pinout.h"
#include "selector.h"
#include "log.h"

ScreenDevice device;
Screen screen;

int* pinout = set_uno_pinout(init_interface);
IODevice io;

int channel;

void on_receive(int n_bytes) {
  int n = n_bytes;
  if (n > (int)sizeof(ScreenState))
    n = (int)sizeof(ScreenState);

  Wire.readBytes((byte*)&io.state.screen, n);
}

void on_request() {
  Wire.write((byte*)&io.state.dial, (int)sizeof(DialState));
}

void setup() {
 Serial.begin(9600);
  log_winks = 10;  // I need a second.
  Serial.println("starting io device");
  Dial dial;
  DialDevice dial_device;

  device.set_pinout(pinout);
  dial_device.set_pinout(pinout);
  dial.attach(&dial_device);
  screen.attach(&device);
  screen.set_point(-1);
  channel = Selector(&dial, &screen).get_channel();

  Serial.print("selected: ");
  Serial.println(channel);
  io.set_pinout(pinout);

  Wire.begin(channel);
  Wire.onReceive(on_receive);
  Wire.onRequest(on_request);
}

void loop() {
  io.update();
  log(io.state);
}