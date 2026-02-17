#include "char_device.h"
#include "dial_encoder.h"
#include "uno_pinout.h"
#include "Wire.h"

const int N_LETTERS = 26;
const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

long mod(long value, long modulus) {  // the year: 1972 (Oscar BP: The French Connection)
  return (value % modulus + modulus) % modulus;
}

char* get_display_word(char* ch, int size, int offset) {
  for (int i = 0; i < size; i++) {
    ch[i] = alphabet[mod(offset + i, N_LETTERS)];
  }
  return ch;
}

CharDevice device;  // local device

CharDisplay* switchboard[0x80];  // 128-address switchboard

const int channels[] = { 0x8, 0x9, 0xA };
const int n_channels = sizeof(channels) / sizeof(int);
const int n_chars = 4 * n_channels;
char display_word[n_chars + 1];
DialEncoder dial;

long controller_index = -1;
const unsigned int banner_ms = 3142;
const long millis_per_step = 2 * banner_ms;

void banner() {
  device.put_str("onir");
  device.set_point(1);
  for (CharDisplay* display : switchboard) {
  }
  while (millis() < banner_ms) {
    device.refresh();
  }
  device.set_point(-1);
  device.clear_str();
}

void setup_channels() {
  for (int channel : channels) {
    switchboard[channel] = new CharDisplay(channel);
  }
}

void refresh_displays() {
  for (int channel : channels) {
    switchboard[channel]->refresh();
  }
}

void setup() {
  Serial.begin(9600);
  int* pinout = set_uno_pinout(init_interface);
  device.set_pinout(pinout);
  dial.set_pinout(pinout);
  setup_channels();
  Serial.println("starting test client.");
  Wire.begin();  // no address == host.
  banner();
}

void update_controller() {
  long step_now = max(0, (millis() - banner_ms)) / millis_per_step;
  dial.update();
  long index_now = step_now + dial.value();
  if (index_now != controller_index) {
    Serial.println(dial.value());
    controller_index = index_now;
    Serial.print("index ");
    Serial.println(index_now);
    for (int i = 0; i < 4; i++) {
      device.put_char(i, (char)((int)'a' + mod(i + index_now, 26)));
    }
    get_display_word(display_word, n_chars, index_now + 4);
    for (int i = 0; i < n_channels; i++) {
      switchboard[channels[i]]->put_str(display_word + 4 * i);
      Serial.println(display_word + 4 * i);
    }
  }
}

void loop() {
  update_controller();
  refresh_displays();
  device.refresh();
}
