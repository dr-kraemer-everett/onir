#pragma once

#include "display.h"
#include "control.h"
#include "log.h"

const static int margin = 256;

// Splits a string across a handful of displays and keeps them refreshed.
class Screen {

public:
  Screen(const Hardware& hardware) : control_(new Control(hardware)) {
    clear();
  }
  
  Screen(Control* control) : control_(control) {
    clear();
  }

  void clear() {
    erase();
    if (fan_start_) {
      fan();
    }
    for (Client* client : control_->clients) {
      if (client) {
        client->dial.zero();
      }
    }
  }

  void fan_start(bool b) {
    fan_start_ = b;
  }
  
  void erase() {
    for (int i = 0; i < margin; i++) {
      display_[i] = 0;
    }
    for (int i = 0; i < BANDS; i++) {
      positions_[i] = 0;
    }
    message_ = 0;
    index_ = 0;
  }
  
  void display(const char* message) {
    message_ = message;
    record();
    show();
  }

  void pan(int move) {
    index_ += move;
    show();
  }

  void seek(int index) {
    index_ = index;
    show();
  }

  int index() const { return index_; }
  int width() const { return length(display_); }

  void show() {
    const int local = control_->local();
    for (int channel = 0; channel < BANDS; channel++) {
      if (Client* client = control_->clients[channel]) {
        for (int i = 0; i < 4; i++) {
          int position = positions_[channel];                      // channel position
          position += i;                                           // digit offset
          position += index_;                                      // pan offset
          if (control_->clients[local]) {
            position += control_->clients[local]->dial.value();    // control offset
          }
          if (channel != local) {
            position += client->dial.value();                      // device offset
          }
          // late-read pattern restricts use of dial value to this spot. (no branches below.)
          client->display.put_char(i, at(position));
        }
      }
    }
  }
  
private:
  char at(int index) {
    return display_[mod(width(), index)];;
  }
  
  void fan() {  // Sequence channel offsets from zero.
    index_ = 0;
    for (int channel = 0; channel < BANDS; channel++) {
      positions_[channel] = 0;
    }
    int count = 0;
    for (int channel = 0; channel < BANDS; channel++) {
      if (control_->clients[channel]) {
        positions_[channel] = 4 * count++;
      }
    }
    show();
  }

  int record() {                                // may pad start/end:
    int receipt = length(message_);             // find the 0
    int length = 0;
    if (receipt and message_[0] != ' ') {
      display_[length++] = ' ';                 // left-pad
    }
    for (int i = 0; (i < receipt) and (length < margin - 1);) {
      display_[length++] = message_[i++];       // copy message
    }
    while (length < 4) {                        // pad to 4
      display_[length++] = ' ';
    }
    if (display_[length-1] != ' '  and length < margin - 2) {
      display_[length++] = ' ';                 // right-pad
    }      
    display_[length] = 0;                       // terminate
    return length;
  }

  static int mod(int modulus, int value) {   // oh, C++...
    int ret = value % modulus;
    if (ret < 0) { ret += modulus; }
    return ret;
  }

  static int length(const char* str) {
    int length = 0;
    while (str and str[length]) { length++; }
    return length;
  }
  
  Control* const control_;
  const char* message_;
  char display_[margin];
  int index_;
  int positions_[BANDS];
  bool fan_start_ = true;

  const Hardware& hardware;  
};
