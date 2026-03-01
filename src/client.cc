#include "client.h"
#include "log.h"
#include "dial_device.h"

#include "Arduino.h"

Client::Client(int channel) : dial(channel), display(channel) { }
  
Client::Client(const Hardware& hardware) : dial(hardware), display(hardware) { }

void Client::update() {
  dial.update();
  display.refresh();
}


