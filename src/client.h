#include "onir.h"
#include "display.h"
#include "dial.h"

class Client {
public:
  Client(const Hardware& hw = no_hardware);
  Client(int channel, const Hardware& hw = no_hardware);

  void update();
  Dial dial;
  Display display;
  int channel;

  IOState local_;  // for logging; use dial and display directly.
private:
  const Hardware& hardware;
};
