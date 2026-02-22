#include "control.h"

#include "Arduino.h"


Control::Control(int* channels, int ct) {
  count_ = ct;
  for (int i = 0; i < BANDS; i++) {
    clients[i] = 0;
  }

  for (int i = 0; i < count_; i++) {
    int channel = channels[i];
    clients[channel] = new Client(channel);
  }
}

void Control::update() {
  for (Client* client : clients) {
    if (client) {
      client->update();
    }
  }
}
