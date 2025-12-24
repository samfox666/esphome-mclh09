#include "mclh_09.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

static const char *const TAG = "mclh_09";

bool MCLH09::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
  if (device.address_uint64() != this->address_) {
    return false;
  }

  ESP_LOGD(TAG, "Found MCLH-09 sensor: %s", device.address_str().c_str());

  const auto &manu_data = device.get_manufacturer_data();
  if (manu_data.size() < 15) {
    ESP_LOGD(TAG, "Data too short (%d)", manu_data.size());
    return false;
  }

  // Ожидаем: FF 90 80 ... или FF 80 ...
  if (manu_data[0] != 0xFF || (manu_data[1] != 0x90 && manu_data[1] != 0x80)) {
    ESP_LOGD(TAG, "Not MCLH-09 data: %02X %02X", manu_data[0], manu_data[1]);
    return false;
  }

  // Пропускаем первые 3 байта: FF 90 80
  size_t offset = 3;

  // Температура: 2 байта, little-endian, в десятых долях °C
  if (this->temperature_sensor_ != nullptr && manu_data.size() >= offset + 2) {
    uint16_t temp = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    float temp_c = temp / 10.0f;
    this->temperature_sensor_->publish_state(temp_c);
    offset += 2;
  } else {
    offset += 2;
  }

  // Влажность: 1 байт (%)
  if (this->humidity_sensor_ != nullptr && manu_data.size() >= offset + 1) {
    float hum = manu_data[offset];
    this->humidity_sensor_->publish_state(hum);
    offset += 1;
  } else {
    offset += 1;
  }

  // Освещённость: 2 байта, little-endian
  if (this->illuminance_sensor_ != nullptr && manu_data.size() >= offset + 2) {
    uint16_t illum = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    this->illuminance_sensor_->publish_state(illum);
    offset += 2;
  } else {
    offset += 2;
  }

  // Влажность почвы: 1 байт (%)
  if (this->soil_moisture_sensor_ != nullptr && manu_data.size() >= offset + 1) {
    float soil = manu_data[offset];
    this->soil_moisture_sensor_->publish_state(soil);
    offset += 1;
  } else {
    offset += 1;
  }

  // Уровень заряда: 1 байт (%)
  if (this->battery_level_sensor_ != nullptr && manu_data.size() >= offset + 1) {
    float bat = manu_data[offset];
    this->battery_level_sensor_->publish_state(bat);
  }

  return true;
}

void MCLH09::dump_config() {
  ESP_LOGCONFIG(TAG, "MCLH-09:");
  ESP_LOGCONFIG(TAG, "  MAC Address: %s", this->address_str().c_str());
}

}  // namespace mclh_09
}  // namespace esphome
