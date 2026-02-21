#include "io_device.h"

IODevice::IODevice() {
  dial = new DialDevice();
  screen = new ScreenDevice();

}
