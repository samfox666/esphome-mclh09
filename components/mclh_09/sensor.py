import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker, sensor
from esphome.const import (
    CONF_ID,
    CONF_MAC_ADDRESS,
    CONF_TEMPERATURE,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)

# Импортируем из __init__.py
from . import MCLH09

# Схема для sensor.mclh_09
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MCLH09),
        cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Устанавливаем MAC-адрес
    cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))

    # Регистрируем сенсор температуры
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))

    # Регистрируем BLE-компонент
    await esp32_ble_tracker.register_ble_component(var, config)
