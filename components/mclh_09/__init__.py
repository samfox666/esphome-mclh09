import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker
from esphome.const import CONF_MAC_ADDRESS, CONF_ID, CONF_DEVICES

CODEOWNERS = ["@samfox666"]
DEPENDENCIES = ["esp32_ble_tracker"]

mclh_09_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_09_ns.class_(
    "MCLH09", esp32_ble_tracker.ESPBTDeviceListener, cg.Component
)

DEVICE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
        cv.Optional("update_interval", default="15min"): cv.update_interval,
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_DEVICES): cv.ensure_list(DEVICE_SCHEMA),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    for device_conf in config[CONF_DEVICES]:
        var = cg.new_Pvariable(device_conf[CONF_ID])
        await cg.register_component(var, device_conf)
        await esp32_ble_tracker.register_ble_device(var, device_conf)
        cg.add(var.set_address(device_conf[CONF_MAC_ADDRESS].as_hex))
        cg.add(var.set_update_interval(device_conf["update_interval"]))
