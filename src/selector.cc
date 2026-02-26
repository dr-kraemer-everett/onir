#include "selector.h"

#include "Arduino.h"

Selector::Selector(const Hardware& hardware) : hardware(hardware) {
  button_down = false;
}

Selector::Selector(Dial* dial, Display* display, bool button=false, const Hardware& hardware)
  : dial(dial), display(display), button_down(button), hardware(hardware) { }

void Selector::channel_up() {
  if (channel < max_channel) {
    channel++;
  } else {
    Serial.print("max channel = ");
    echo();
  }
}

void Selector::channel_down() {
  if (channel > min_channel)  {
    channel--;
  } else {
    Serial.println();
    Serial.print("min channel = ");
    echo();
  }
}

char Selector::hex_digit(int value) {
  if ((value < 0) or (value > 16)) return '-';
  if (value < 10) return '0' + value;
  return 'A' + value - 10;
}

void Selector::display_channel() {
  display->put_char(0, '0');
  display->put_char(1, 'x');
  display->put_char(2, hex_digit(channel / 16));
  display->put_char(3, hex_digit(channel % 16));
}


int Selector::get_channel() {
  Serial.print("channel: ");
  Dial local_dial(hardware);;
  display_channel();
  long value = dial->value();
  dial->update();
  while (not done()) {
    dial->update();
    if (count() != value) {
      if (count() > value) {
        channel_up();
      }
      if (count() < value) {
        channel_down();
      }
      value = count();
      
      display_channel();
      echo();
      Serial.print("? ");
    }
    display->refresh();
  }
  echo();
  Serial.println(".");
  Serial.print("channel ");
  Serial.println(channel);
  dial->zero();
  return channel;  
}

int Selector::count() {
  if (button_down) {
    Serial.println(button_down);
    return dial->down_value();
  } else {
    return dial->value();
  }
}

bool Selector::done() {
  if (button_down) {
    return dial->release();
  } else {
    return dial->press();
  }
}

void Selector::echo() {
  for (int i = 0; i < 4; i++) {
    Serial.print(display->state.chars[i]);
  }
}
