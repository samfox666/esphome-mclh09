import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client
from esphome.const import CONF_ID, CONF_BLE_CLIENT_ID

# Namespace
mclh_ns = cg.esphome_ns.namespace("mclh_09")
MCLH09 = mclh_ns.class_("MCLH09", cg.Component)

# Схема
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_BLE_CLIENT_ID): cv.use_id(ble_client.BLEClient),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    paren = await cg.get_variable(config[CONF_BLE_CLIENT_ID])
    cg.add(var.set_ble_client_parent(paren))
    
