import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import CONF_ID, CONF_MAC_ADDRESS

# Namespace
mclh_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_ns.class_("MCLH09", cg.Component)

# Определяем схему конфигурации
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
    }
).extend(cv.COMPONENT_SCHEMA)

# Основная функция для генерации кода
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Устанавливаем MAC-адрес
    cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))

    # Регистрируем BLE-компонент (новый способ)
    cg.add(esp32_ble_tracker.global_esp32_ble_tracker.add_listener(var))

# Экспортируем MCLH09 и mclh_ns для sensor.py
mclh_09_ns = mclh_ns
