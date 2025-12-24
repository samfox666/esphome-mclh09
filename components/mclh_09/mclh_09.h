#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/ble_client/ble_client.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

class MCLH09 : public Component, public ble_client::BLEClientNode {
 public:
  void set_address(uint64_t address) { address_ = address; }
  void set_temperature_sensor(sensor::Sensor* temperature) { temperature_sensor_ = temperature; }
  void set_humidity_sensor(sensor::Sensor* humidity) { humidity_sensor_ = humidity; }
  void set_illuminance_sensor(sensor::Sensor* illuminance) { illuminance_sensor_ = illuminance; }
  void set_soil_moisture_sensor(sensor::Sensor* soil_moisture) { soil_moisture_sensor_ = soil_moisture; }
  void set_battery_level_sensor(sensor::Sensor* battery_level) { battery_level_sensor_ = battery_level; }

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Новый метод — вызывается при BLE-обнаружении
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;

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
