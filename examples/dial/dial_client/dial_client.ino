#include "Wire.h"

#include "data.h"
#include "channel.h"
#include "log.h"
#include "dial/dial.h"

Dial* dial;
Reading prior{};

const int channel = number(Channel::dial);

void setup() {
  Serial.begin(9600);
  Serial.println("dial client " + String(channel));

  dial = new Dial(channel);
  Wire.begin();  // client mode
  memcheck();
}

void loop() {
  dial->update();
  if (dial->reading != prior) {
    if (dial->press()) Serial.println("press");
    if (dial->release()) Serial.println("release");
    print_reading(prior = dial->reading);
  }
}