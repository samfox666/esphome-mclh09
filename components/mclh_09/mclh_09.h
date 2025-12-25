#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

class MCLH09 : public Component, public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  MCLH09(const std::vector<uint64_t> &mac_addresses, uint32_t update_interval = 3600000, bool error_counting = false, bool raw_soil = false);

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Новый метод — вызывается при BLE-обнаружении
  void on_ble_advertise(esp32_ble_tracker::ESPBTDevice &device) override;

 protected:
  std::vector<uint64_t> mac_addresses_;
  size_t device_count_;

  // Указатели на массивы сенсоров
  sensor::Sensor **temperature_sensor_{nullptr};
  sensor::Sensor **humidity_sensor_{nullptr};
  sensor::Sensor **illuminance_sensor_{nullptr};
  sensor::Sensor **soil_moisture_sensor_{nullptr};
  sensor::Sensor **battery_level_sensor_{nullptr};

  // Метод для публикации данных
  void publish_data(size_t index, const std::vector<uint8_t> &data);
};

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
