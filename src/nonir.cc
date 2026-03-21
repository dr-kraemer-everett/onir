#include "nonir.h"

Nonir::Nonir(int* channels, int count, const Hardware& hw) : hardware(hw) {
// Nonir::Nonir(int* channels, int count) {
  control = new Control(channels, count);
  screen = new Screen(control);
}

