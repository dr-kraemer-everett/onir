#include <Wire.h>

#include "onir.h"
#include "dial_device.h"

DialDevice device;
DialState state;

int pinout[(int)PinFunction::END];

const int I2C_ADDRESS = 8;

long last_count = 0;
bool last_button = false;

void on_request() {
  Wire.write((byte*)&state, sizeof(DialState));
}

void setup() {
  Serial.begin(9600);
  Serial.println("dial device start");
  Serial.print("i2c address: ");
  Serial.println(I2C_ADDRESS);

  pinout[(int)PinFunction::CLOCK] = 17;   // A3
  pinout[(int)PinFunction::DATA] = 16;    // A2
  pinout[(int)PinFunction::SWITCH] = 15;  // A1

  device.set_pinout(pinout);
  device.init();

  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(on_request);
}

void loop() {
  device.read(state);

  if (state.count != last_count || state.button != last_button) {
    Serial.print("count: ");
    Serial.print(state.count);
    Serial.print("  button: ");
    Serial.println(state.button);

    last_count = state.count;
    last_button = state.button;
  }
}
