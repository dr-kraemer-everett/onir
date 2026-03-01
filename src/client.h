#include "onir.h"
#include "display.h"
#include "dial.h"

class Client {
public:
  Client(const Hardware& hardware);
  Client(int channel);

  void update();
  Dial dial;
  Display display;
  int channel;

  IOState local_;  // for logging; use dial and display directly.
};
