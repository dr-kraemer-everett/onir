#include "char_device.h"
#include "dial_encoder.h"
#include "uno_pinout.h"
#include "channel_selector.h"

#include "Wire.h"

DialEncoder dial;
CharDevice device;

int channel = -1;

const unsigned int channel_display_ms = 1600;

// for setup
bool channel_display = true;
int dial_value = 0;
long start_millis = -1;
bool recieved = false;

bool running() {
  return start_millis >= 0;
}

void clear_channel() {
  if (not running()) return;        // still selecting channel.
  if (not channel_display) return;  // already cleared.
  if (recieved) {                   // already in use.
    channel_display = false;
    return;
  }
  if (millis() > start_millis + channel_display_ms) {
    device.clear_str();
    channel_display = false;
  }
}

void update_display_state(int message_size) {
  Wire.readBytes((byte*)&device.state, message_size);
  recieved = true;
}

void init_channel(int channel) {
  Serial.println("channel " + String(channel));
  Wire.begin(channel);
  start_millis = millis();
  Serial.println("start ms " + String(start_millis));

  // go.
  Wire.onReceive(update_display_state);
}

void check_cleanup() {
  if (not running()) return;  // not yet.
  clear_channel();
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting character display.");
  Serial.println("start ms: " + String(start_millis));
  int* pinout = set_uno_pinout(init_interface);
  device.set_pinout(pinout);
  dial.set_pinout(pinout);
  init_channel(ChannelSelector(&dial, &device).get_channel());
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
