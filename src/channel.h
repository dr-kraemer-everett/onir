#pragma once

enum class Channel : int {
  dial      = 0x08,
  display   = 0x09,
  car       = 0x0A,
  screen_1  = 0x0B,
  screen_2  = 0x0C,
  screen_3  = 0x0D,
};

inline int number(Channel channel) {
  return (int)channel;
}
