#include "control.h"

#include "Arduino.h"

Control::Control(int* ch, int count) {
  channels = ch;
  for (int i = 0; i < BANDS; i++) {
    clients[i] = nullptr;
  }

  for (int i = 0; i < count; i++) {
    clients[channels[i]] = new Client(channels[i]);
  }
}

void Control::update() {
    for (Client* client : clients) {
    if (client) {
      client->update();
    }
  }
}
