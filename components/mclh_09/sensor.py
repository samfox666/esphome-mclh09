import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_HUMIDITY,
    CONF_ILLUMINANCE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_LUX,
)

from . import MCLH09, mclh_09_ns

DEPENDENCIES = ["mclh_09"]

CONF_MCLH_09_ID = "mclh_09_id"
CONF_SOIL_MOISTURE = "soil_moisture"
CONF_UPDATE_INTERVAL = "update_interval"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MCLH_09_ID): cv.use_id(MCLH09),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(),
        cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(),
        cv.Optional(CONF_SOIL_MOISTURE): sensor.sensor_schema(),
        cv.Optional(CONF_ILLUMINANCE): sensor.sensor_schema(),
        cv.Optional(CONF_UPDATE_INTERVAL, default="15min"): cv.update_interval,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MCLH_09_ID])

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(parent.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(parent.set_humidity_sensor(sens))
    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(parent.set_battery_level_sensor(sens))
    if CONF_SOIL_MOISTURE in config:
        sens = await sensor.new_sensor(config[CONF_SOIL_MOISTURE])
        cg.add(parent.set_soil_moisture_sensor(sens))
    if CONF_ILLUMINANCE in config:
        sens = await sensor.new_sensor(config[CONF_ILLUMINANCE])
        cg.add(parent.set_illuminance_sensor(sens))

    cg.add(parent.set_update_interval(config[CONF_UPDATE_INTERVAL]))
