import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker, myhomeiot_ble_host
from esphome.const import (
    CONF_ID,
    CONF_MAC_ADDRESS,
)

# Namespace
mclh_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_ns.class_("MCLH09", cg.Component)

# Определяем схему конфигурации
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_MAC_ADDRESS): cv.ensure_list(cv.mac_address),
        cv.Optional(CONF_ERROR_COUNTING, default=False): cv.boolean,
        cv.Optional(CONF_RAW_SOIL, default=False): cv.boolean,
        cv.Required(CONF_BLE_HOST): cv.use_id(myhomeiot_ble_host.MyHomeIOT_BLEHost),
    }
).extend(cv.COMPONENT_SCHEMA)

# Основная функция для генерации кода
async def to_code(config):
    addr_list = []
    for it in config[CONF_MAC_ADDRESS]:
      addr_list.append(it.as_hex)
    var = cg.new_Pvariable(config[CONF_ID], addr_list, config[CONF_ERROR_COUNTING], config[CONF_RAW_SOIL])
    ble_host = await cg.get_variable(config[CONF_BLE_HOST])
    cg.add(var.set_ble_host(ble_host))
    await cg.register_component(var, config)

    # Регистрируем BLE-компонент
    cg.add(esp32_ble_tracker.global_esp32_ble_tracker.add_listener(var))
