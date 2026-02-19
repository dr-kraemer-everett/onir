#include "selector.h"

void Selector::channel_up() {
  if (channel < max_channel) channel++;
}

void Selector::channel_down() {
  if (channel > min_channel) channel--;
}

char Selector::hex_digit(int value) {
  if ((value < 0) or (value > 16)) return '-';
  if (value < 10) return '0' + value;
  return 'A' + value - 10;
}

void Selector::display_channel() {
  screen->put_char(0, '0');
  screen->put_char(1, 'x');
  screen->put_char(2, hex_digit(channel / 16));
  screen->put_char(3, hex_digit(channel % 16));
}


int Selector::get_channel() {
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
