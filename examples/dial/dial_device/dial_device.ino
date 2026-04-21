#include "Wire.h"

#include "channel.h"
#include "circuits.h"
#include "log.h"

#include "dial/dial_device.h"

DialDevice* dial;
Hardware hardware {};
const int channel = number(Channel::dial);

Reading reading {};
Reading prior {};

void on_request() {
  Wire.write((byte*)&reading, sizeof(Reading));
}

void setup() {
  Serial.begin(9600);
  Serial.println("dial start");

  uno_io(hardware);
  dial = new DialDevice(hardware);

  Serial.print("I2C channel ");
  Serial.println(channel);

  Wire.begin(channel);
  Wire.onRequest(on_request);
}

void loop() {
  dial->take(reading);
//  if (reading.count != count || reading.down_count != down_count || reading.button != button) {
  if (reading != prior) {
    prior = reading;
    print_reading(reading);
  }
}
