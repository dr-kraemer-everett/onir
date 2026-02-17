#include "channel_selector.h"

void ChannelSelector::channel_up() {
  if (channel < max_channel) channel++;
}

void ChannelSelector::channel_down() {
  if (channel > min_channel) channel--;
}

char ChannelSelector::hex_digit(int value) {
  if ((value < 0) or (value > 16)) return '-';
  if (value < 10) return '0' + value;
  return 'A' + value - 10;
}

void ChannelSelector::display_channel() {
  screen->put_char(0, '0');
  screen->put_char(1, 'f');
  screen->put_char(2, hex_digit(channel / 16));
  screen->put_char(3, hex_digit(channel % 16));
}


int ChannelSelector::get_channel() {
  Serial.println("enter");
  dial->zero();
  display_channel();
  long dial_value = dial->value();
  dial->update();
  while (not dial->signal()) {
    dial->update();
    if (dial->value() != dial_value) {
      Serial.println(dial->value());
      if (dial->value() > dial_value) {
        channel_up();
      }
      if (dial->value() < dial_value) {
        channel_down();
      }
      dial_value = dial->value();
      
      display_channel();
    }
    screen->refresh();
  }
  Serial.println("selected: " + String(channel));
  dial->zero();
  return channel;  
}
