#include "mclh_09.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

static const char *const TAG = "mclh_09";

void MCLH09::on_ble_advertise(esp32_ble_tracker::ESPBTDevice &device) {
  // Проверяем MAC-адрес
  if (device.address_uint64() != this->address_) {
    return;
  }

  ESP_LOGD(TAG, "Found MCLH-09 sensor: %s", device.address_str().c_str());

  // Получаем manufacturer data
  const auto &manu_data = device.get_manufacturer_data();
  if (manu_data.size() < 15) {
    ESP_LOGD(TAG, "Data too short (%d)", manu_data.size());
    return;
  }

  if (manu_data[0] != 0xFF || (manu_data[1] != 0x90 && manu_data[1] != 0x80)) {
    ESP_LOGD(TAG, "Not MCLH-09  %02X %02X", manu_data[0], manu_data[1]);
    return;
  }

  size_t offset = 3;

  // Температура: 2 байта, little-endian, в десятых долях °C
  if (this->temperature_sensor_ != nullptr && offset + 2 <= manu_data.size()) {
    uint16_t temp = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    this->temperature_sensor_->publish_state(temp / 10.0f);
    offset += 2;
  } else {
    offset += 2;
  }

  // Влажность: 1 байт (%)
  if (this->humidity_sensor_ != nullptr && offset < manu_data.size()) {
    this->humidity_sensor_->publish_state(manu_data[offset]);
    offset += 1;
  } else {
    offset += 1;
  }

  // Освещённость: 2 байта, little-endian
  if (this->illuminance_sensor_ != nullptr && offset + 2 <= manu_data.size()) {
    uint16_t illum = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    this->illuminance_sensor_->publish_state(illum);
    offset += 2;
  } else {
    offset += 2;
  }

  // Влажность почвы: 1 байт (%)
  if (this->soil_moisture_sensor_ != nullptr && offset < manu_data.size()) {
    this->soil_moisture_sensor_->publish_state(manu_data[offset]);
    offset += 1;
  } else {
    offset += 1;
  }

  // Уровень заряда: 1 байт (%)
  if (this->battery_level_sensor_ != nullptr && offset < manu_data.size()) {
    this->battery_level_sensor_->publish_state(manu_data[offset]);
  }
}

void MCLH09::dump_config() {
  ESP_LOGCONFIG(TAG, "MCLH-09:");
  ESP_LOGCONFIG(TAG, "  MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
                (uint8_t)(address_ >> 40) & 0xFF,
                (uint8_t)(address_ >> 32) & 0xFF,
                (uint8_t)(address_ >> 24) & 0xFF,
                (uint8_t)(address_ >> 16) & 0xFF,
                (uint8_t)(address_ >> 8) & 0xFF,
                (uint8_t)(address_) & 0xFF);
}

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
