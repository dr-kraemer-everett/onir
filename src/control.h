#include "client.h"

#define BANDS 16

class Control {
public:
  Control(int* channels, int count);
  void update();
  Client* clients[BANDS];
  
private:
  int* channels = nullptr;
};


  
