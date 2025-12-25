#include "mclh_09.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

static const char *const TAG = "mclh_09";

#define SENSOR_ID "mclh09_%03d_%s"
#define SENSOR_NAME "mclh09_%03d %s"

MCLH09::MCLH09(const std::vector<uint64_t> &mac_addresses, uint32_t update_interval, bool error_counting, bool raw_soil)
    : mac_addresses_(mac_addresses), device_count_(mac_addresses_.size()) {
  // Выделяем память под массивы сенсоров
  temperature_sensor_ = new sensor::Sensor *[device_count_];
  humidity_sensor_ = new sensor::Sensor *[device_count_];
  illuminance_sensor_ = new sensor::Sensor *[device_count_];
  soil_moisture_sensor_ = new sensor::Sensor *[device_count_];
  battery_level_sensor_ = new sensor::Sensor *[device_count_];

  // создаём связи между ble-клиентами и сенсорами и регистрируем всё в Esphome
  char buffer[64];
  for (size_t i = 0; i < device_count_; i++) {
    // датчик Battery Level
    battery_level_sensor_[i] = new sensor::Sensor();
    snprintf(buffer, sizeof(buffer), SENSOR_NAME, i + 1, "batt");
    battery_level_sensor_[i]->set_name(strdup(buffer));
    snprintf(buffer, sizeof(buffer), SENSOR_ID, i + 1, "batt");
    battery_level_sensor_[i]->set_object_id(strdup(buffer));
    battery_level_sensor_[i]->set_disabled_by_default(false);
    battery_level_sensor_[i]->set_device_class("battery");
    battery_level_sensor_[i]->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
    battery_level_sensor_[i]->set_unit_of_measurement("%");
    battery_level_sensor_[i]->set_accuracy_decimals(0);
    battery_level_sensor_[i]->set_force_update(false);
    App.register_sensor(battery_level_sensor_[i]);

    // датчик Temperature
    temperature_sensor_[i] = new sensor::Sensor();
    snprintf(buffer, sizeof(buffer), SENSOR_NAME, i + 1, "temp");
    temperature_sensor_[i]->set_name(strdup(buffer));
    snprintf(buffer, sizeof(buffer), SENSOR_ID, i + 1, "temp");
    temperature_sensor_[i]->set_object_id(strdup(buffer));
    temperature_sensor_[i]->set_disabled_by_default(false);
    temperature_sensor_[i]->set_device_class("temperature");
    temperature_sensor_[i]->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
    temperature_sensor_[i]->set_unit_of_measurement("°C");
    temperature_sensor_[i]->set_accuracy_decimals(1);
    temperature_sensor_[i]->set_force_update(false);
    App.register_sensor(temperature_sensor_[i]);

    // датчик Humidity
    humidity_sensor_[i] = new sensor::Sensor();
    snprintf(buffer, sizeof(buffer), SENSOR_NAME, i + 1, "humid");
    humidity_sensor_[i]->set_name(strdup(buffer));
    snprintf(buffer, sizeof(buffer), SENSOR_ID, i + 1, "humid");
    humidity_sensor_[i]->set_object_id(strdup(buffer));
    humidity_sensor_[i]->set_disabled_by_default(false);
    humidity_sensor_[i]->set_device_class("humidity");
    humidity_sensor_[i]->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
    humidity_sensor_[i]->set_unit_of_measurement("%");
    humidity_sensor_[i]->set_accuracy_decimals(0);
    humidity_sensor_[i]->set_force_update(false);
    App.register_sensor(humidity_sensor_[i]);

    // датчик Illuminance
    illuminance_sensor_[i] = new sensor::Sensor();
    snprintf(buffer, sizeof(buffer), SENSOR_NAME, i + 1, "illum");
    illuminance_sensor_[i]->set_name(strdup(buffer));
    snprintf(buffer, sizeof(buffer), SENSOR_ID, i + 1, "illum");
    illuminance_sensor_[i]->set_object_id(strdup(buffer));
    illuminance_sensor_[i]->set_disabled_by_default(false);
    illuminance_sensor_[i]->set_device_class("illuminance");
    illuminance_sensor_[i]->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
    illuminance_sensor_[i]->set_unit_of_measurement("lx");
    illuminance_sensor_[i]->set_accuracy_decimals(0);
    illuminance_sensor_[i]->set_force_update(false);
    App.register_sensor(illuminance_sensor_[i]);

    // датчик Soil Moisture
    soil_moisture_sensor_[i] = new sensor::Sensor();
    snprintf(buffer, sizeof(buffer), SENSOR_NAME, i + 1, "soil");
    soil_moisture_sensor_[i]->set_name(strdup(buffer));
    snprintf(buffer, sizeof(buffer), SENSOR_ID, i + 1, "soil");
    soil_moisture_sensor_[i]->set_object_id(strdup(buffer));
    soil_moisture_sensor_[i]->set_disabled_by_default(false);
    soil_moisture_sensor_[i]->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
    soil_moisture_sensor_[i]->set_unit_of_measurement("%");
    soil_moisture_sensor_[i]->set_accuracy_decimals(0);
    soil_moisture_sensor_[i]->set_force_update(false);
    App.register_sensor(soil_moisture_sensor_[i]);
  }
}

void MCLH09::on_ble_advertise(esp32_ble_tracker::ESPBTDevice &device) {
  // Проверяем MAC-адрес
  uint64_t addr = device.address_uint64();
  for (size_t i = 0; i < device_count_; i++) {
    if (addr == mac_addresses_[i]) {
      // Получаем manufacturer data
      const auto &manu_data = device.get_manufacturer_data();
      if (manu_data.size() >= 15) {
        if (manu_data[0] == 0xFF && (manu_data[1] == 0x90 || manu_data[1] == 0x80)) {
          size_t offset = 3;

          // Температура
          if (offset + 2 <= manu_data.size()) {
            uint16_t temp = encode_uint16(manu_data[offset + 1], manu_data[offset]);
            if (temperature_sensor_[i] != nullptr) {
              temperature_sensor_[i]->publish_state(temp / 10.0f);
            }
            offset += 2;
          } else {
            offset += 2;
          }

          // Влажность
          if (offset < manu_data.size()) {
            if (humidity_sensor_[i] != nullptr) {
              humidity_sensor_[i]->publish_state(manu_data[offset]);
            }
            offset += 1;
          } else {
            offset += 1;
          }

          // Освещённость
          if (offset + 2 <= manu_data.size()) {
            uint16_t illum = encode_uint16(manu_data[offset + 1], manu_data[offset]);
            if (illuminance_sensor_[i] != nullptr) {
              illuminance_sensor_[i]->publish_state(illum);
            }
            offset += 2;
          } else {
            offset += 2;
          }

          // Влажность почвы
          if (offset < manu_data.size()) {
            if (soil_moisture_sensor_[i] != nullptr) {
              soil_moisture_sensor_[i]->publish_state(manu_data[offset]);
            }
            offset += 1;
          } else {
            offset += 1;
          }

          // Уровень заряда
          if (offset < manu_data.size()) {
            if (battery_level_sensor_[i] != nullptr) {
              battery_level_sensor_[i]->publish_state(manu_data[offset]);
            }
          }
        }
      }
      break; // Нашли устройство, выходим из цикла
    }
  }
}

void MCLH09::dump_config() {
  ESP_LOGCONFIG(TAG, "MCLH-09:");
  for (size_t i = 0; i < device_count_; i++) {
    ESP_LOGCONFIG(TAG, "  Device %d MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
                  i + 1,
                  (uint8_t)(mac_addresses_[i] >> 40) & 0xFF,
                  (uint8_t)(mac_addresses_[i] >> 32) & 0xFF,
                  (uint8_t)(mac_addresses_[i] >> 24) & 0xFF,
                  (uint8_t)(mac_addresses_[i] >> 16) & 0xFF,
                  (uint8_t)(mac_addresses_[i] >> 8) & 0xFF,
                  (uint8_t)(mac_addresses_[i]) & 0xFF);
  }
}

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
