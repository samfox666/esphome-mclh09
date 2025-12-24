import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import CONF_MAC_ADDRESS, CONF_ID

CODEOWNERS = ["@yourname"]
DEPENDENCIES = ["esp32_ble_tracker"]

mclh_09_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_09_ns.class_(
    "MCLH09", esp32_ble_tracker.ESPBTDeviceListener, cg.Component
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MCLH09),
            cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await esp32_ble_tracker.register_ble_device(var, config)
    cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))
