#include "client.h"

#include "Arduino.h"

Client::Client(int channel) : dial(channel), screen(channel) {
}

void Client::update() {
  dial.update();
  screen.refresh();
}


