#pragma once

#include "client.h"

#define BANDS 16      // first eight spots unused. (https://i2cdevices.org/addresses)
#define LOCAL_BAND 0  // zero = here  

class Control {
public:
  Control(int* channels, int count);

  void update();
  int count() {return count_;}

  Client* clients[BANDS];
 
private:
  int count_ = 0;
};


  
