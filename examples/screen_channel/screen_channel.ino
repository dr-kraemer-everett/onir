#include "screen.h"
#include "screen_device.h"
#include "dial.h"
#include "dial_device.h"
#include "uno_pinout.h"
#include "selector.h"
#include "onir.h"

#include "Wire.h"

ScreenDevice device;
Screen screen;

int channel;

const unsigned int channel_display_ms = 1600;
const unsigned int default_channel_start_ms = 3 * channel_display_ms;

// for setup
int dial_value = 0;
bool channel_display = true;
bool recieved = false;

long start_millis = -1;
bool running() {
  return start_millis >= 0;
}

void clear() {
  if (not channel_display) return;  // already cleared.
  if (recieved) {                   // already in use.
    channel_display = false;
    return;
  }
  if (millis() > start_millis + channel_display_ms) {
    device.clear();
    channel_display = false;
  }
}

void update_screen(int message_size) {
  Wire.readBytes((byte*)&device.state, message_size);
  recieved = true;
}

void init_channel(int channel) {
  start_millis = millis();
  Serial.print("channel ");
  Serial.print(channel);
  Serial.print(" start ms: ");
  Serial.println(start_millis);

  Wire.begin(channel);            // ready
  Wire.onReceive(update_screen);  // go.
}

void check_cleanup() {
  if (not running()) return;  // not yet.
  clear();
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting character display.");
  Serial.println("start ms: " + String(start_millis));
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
  init_channel(channel);
}

long steps = -1;
void log_status() {
  long prior_steps = steps;
  steps = millis() / channel_display_ms;
  if (prior_steps != steps) {
    Serial.println("c: " + String(channel));
  }
}

void loop() {
  check_cleanup();
  device.refresh();
  log_status();
}
