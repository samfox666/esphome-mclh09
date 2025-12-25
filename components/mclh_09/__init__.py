import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import (
    CONF_ID,
)

# Namespace
mclh_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_ns.class_("MCLH09", cg.Component)

# Определяем схему конфигурации
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required("mac_addresses"): cv.ensure_list(cv.mac_address),  # <-- строка
    }
).extend(cv.COMPONENT_SCHEMA)

# Основная функция для генерации кода
async def to_code(config):
    mac_addrs = [cg.RawExpression(f"0x{addr.replace(':', '')}") for addr in config["mac_addresses"]]  # <-- строка
    
    var = cg.new_Pvariable(config[CONF_ID], mac_addrs)
    await cg.register_component(var, config)

    # Регистрируем BLE-компонент
    cg.add(esp32_ble_tracker.global_esp32_ble_tracker.add_listener(var))
