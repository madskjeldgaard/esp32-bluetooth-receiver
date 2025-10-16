#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <Arduino.h>
#include "esp_bt.h"
#include "esp_gap_bt_api.h"
#include <ezButton.h>


I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

constexpr auto name = "HildegardVonBangerz";

// setup button - FIXED: Use Button instead of Bounce
constexpr auto button_pin = 13; // GPIO0, change as needed
ezButton button(button_pin);

// Alternative pin names:
// PCM5102A:
// - WS --> LCK
// - BCK --> BCK
// - DATA --> DIN

#ifdef USE_WS_PIN
constexpr auto ws_pin = USE_WS_PIN;
#else
#pragma message("Using default WS pin 15")
constexpr auto ws_pin = 15;
#endif

#ifdef USE_BCK_PIN
constexpr auto bck_pin = USE_BCK_PIN;
#else
#pragma message("Using default BCK pin 14")
constexpr auto bck_pin = 14;
#endif

#ifdef USE_DATA_PIN
constexpr auto data_pin = USE_DATA_PIN;
#else
#pragma message("Using default DATA pin 22")
constexpr auto data_pin = 22;
#endif

constexpr unsigned long pairable_duration_ms = 3 * 60 * 1000;

bool pairable = false;
unsigned long pairable_start = 0;

void setPairable(bool enable) {
  pairable = enable;
  if (enable) {
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    pairable_start = millis();
  } else {
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
  }
}

void setup() {
  // Configure the button
    button.setDebounceTime(50); // 50ms debounce

  auto cfg = i2s.defaultConfig();
  cfg.pin_bck = bck_pin;
  cfg.pin_ws = ws_pin;
  cfg.pin_data = data_pin;
  i2s.begin(cfg);

  a2dp_sink.start(name);
  setPairable(false);
}

void loop() {
    button.loop();

  if (button.isPressed()||button.isReleased()) {
    setPairable(true);
  }

  const auto pairableTimeHasPassed = millis() - pairable_start > pairable_duration_ms;
  if (pairable && pairableTimeHasPassed) {
    setPairable(false);
  }
}
