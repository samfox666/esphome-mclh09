import esphome.codegen as cg
from esphome.components import esp32_ble_tracker
import esphome.config_validation as cv
from esphome.const import CONF_MAC_ADDRESS, CONF_ID

AUTO_LOAD = ["esp32_ble_tracker"]

mclh_09_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_09_ns.class_(
    "MCLH09", esp32_ble_tracker.ESPBTDeviceListener, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # ✅ Правильная регистрация слушателя для ESPHome 2025
    cg.add(esp32_ble_tracker.global_esp32_ble_tracker.add_listener(var))

    cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))
    
