#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <Arduino.h>

I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

constexpr auto name = "Casartinos";

void setup() {
  auto cfg = i2s.defaultConfig();

  // Called BCK on PCM5102a boards
  cfg.pin_bck = 14;
  // WS is called LRCK on PCM5102a boards
  cfg.pin_ws = 15;
  // data pin is called DIN on PCM5102a boards
  cfg.pin_data = 22;

  i2s.begin(cfg);

  a2dp_sink.start(name);
}

void loop() {}
