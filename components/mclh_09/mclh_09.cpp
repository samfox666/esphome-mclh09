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

  if (manu_data[0] != 0xFF || (manu_data[1] != 0x90 && manu_data[1] != 0x80)) {
    ESP_LOGD(TAG, "Not MCLH-09 data: %02X %02X", manu_data[0], manu_data[1]);
    return false;
  }

  size_t offset = 3;

  if (this->temperature_sensor_ != nullptr && offset + 2 <= manu_data.size()) {
    uint16_t temp = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    this->temperature_sensor_->publish_state(temp / 10.0f);
    offset += 2;
  } else {
    offset += 2;
  }

  if (this->humidity_sensor_ != nullptr && offset < manu_data.size()) {
    this->humidity_sensor_->publish_state(manu_data[offset]);
    offset += 1;
  } else {
    offset += 1;
  }

  if (this->illuminance_sensor_ != nullptr && offset + 2 <= manu_data.size()) {
    uint16_t illum = encode_uint16(manu_data[offset + 1], manu_data[offset]);
    this->illuminance_sensor_->publish_state(illum);
    offset += 2;
  } else {
    offset += 2;
  }

  if (this->soil_moisture_sensor_ != nullptr && offset < manu_data.size()) {
    this->soil_moisture_sensor_->publish_state(manu_data[offset]);
    offset += 1;
  } else {
    offset += 1;
  }

  if (this->battery_level_sensor_ != nullptr && offset < manu_data.size()) {
    this->battery_level_sensor_->publish_state(manu_data[offset]);
  }

  return true;
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
