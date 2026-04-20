#include "Arduino.h"

#include "Wire.h"

#include "circuits.h"
#include "motor/driver.h"
#include "log.h"

Hardware hardware = {};
Driver* driver{};

Instruction instruction{};

void take_instruction(int message_size) {
  Wire.readBytes((byte*)&instruction, sizeof(Instruction));
}

void send_outcome() {
  if (empty(instruction)  // nothing yet
      or instruction) {   // still working
    Wire.write((const byte*)&instruction, sizeof(Loop));
  } else {
    Wire.write((const byte*)&instruction, sizeof(Instruction));
  }
}

const int channel = 0x09;

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  driver = new Driver(hardware);

  Serial.print("driver (channel ");
  Serial.print(channel);

  Wire.begin(channel);
  Wire.onReceive(take_instruction);
  Wire.onRequest(send_outcome);

  Serial.print("start (~");
  Serial.print(gamut<Instruction>());
  Serial.println(" free):");
}

void loop() {
  if (instruction) {
    driver->follow(instruction);
    print_instruction(instruction);
  } else {
    driver->drive();
  }
}
