#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32

#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

namespace esphome {
namespace mclh_09 {

class MCLH09 : public esp32_ble_tracker::ESPBTDeviceListener, public Component {
 public:
  void set_address(uint64_t address) { address_ = address; }
  void set_temperature_sensor(sensor::Sensor* temperature) { temperature_sensor_ = temperature; }
  void set_humidity_sensor(sensor::Sensor* humidity) { humidity_sensor_ = humidity; }
  void set_illuminance_sensor(sensor::Sensor* illuminance) { illuminance_sensor_ = illuminance; }
  void set_soil_moisture_sensor(sensor::Sensor* soil_moisture) { soil_moisture_sensor_ = soil_moisture; }
  void set_battery_level_sensor(sensor::Sensor* battery_level) { battery_level_sensor_ = battery_level; }

  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  uint64_t address_{0};
  sensor::Sensor* temperature_sensor_{nullptr};
  sensor::Sensor* humidity_sensor_{nullptr};
  sensor::Sensor* illuminance_sensor_{nullptr};
  sensor::Sensor* soil_moisture_sensor_{nullptr};
  sensor::Sensor* battery_level_sensor_{nullptr};
};

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
