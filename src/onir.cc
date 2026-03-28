#include "onir.h"
#include "control.h"
#include "screen.h"

#include "Servo.h"

#include "Arduino.h"

Onir::Onir(int* channels, int count, const Hardware& hardware) : hardware(hardware) {
  control = new Control(channels, count, hardware);
  Serial.println("frotz");
  Serial.println((int)control);
  screen = new Screen(control);
  Serial.println("qux");
  Serial.println((int)screen);
  Servo* servo = new Servo();
  Serial.println((int)servo);
  delay(100);
}

void Onir::display(char* message) {
  screen->display(message);
}

void blank(Message* message) {
  for (int i = 0; i < 4; i++) {
    message->chars[i] = ' ';
  }
}

void Onir::act() {
  if (go()) {
    screen->pan(1);
    log_io(control->local());
  }
}

void Onir::update() {
  control->check();
  act();
  screen->show();
}

int Onir::now() {
  return millis() / step_ms_;
}

bool Onir::go() {
  int past = now_;
  now_ = now();
  return past != now_;
}
