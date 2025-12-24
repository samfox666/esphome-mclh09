#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

class MCLH09 : public esp32_ble_tracker::ESPBTDeviceListener, public Component {
 public:
  void set_address(uint64_t address) { address_ = address; }
  void set_temperature_sensor(sensor::Sensor* temperature) { temperature_sensor_ = temperature; }

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Старый метод — вызывается при BLE-обнаружении
  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;

 protected:
  uint64_t address_{0};
  sensor::Sensor* temperature_sensor_{nullptr};
};

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
