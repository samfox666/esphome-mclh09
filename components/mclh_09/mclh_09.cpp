#include "mclh_09.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace mclh_09 {

static const char *const TAG = "mclh_09";

void MCLH09::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                 esp_ble_gattc_cb_param_t *param) {
  // Обработка BLE-событий
  // Пока пусто — MCLH-09 работает через Advertising
}

void MCLH09::dump_config() {
  ESP_LOGCONFIG(TAG, "MCLH-09:");
  // Печатаем MAC-адрес, если есть
}

}  // namespace mclh_09
}  // namespace esphome

#endif  // USE_ESP32
