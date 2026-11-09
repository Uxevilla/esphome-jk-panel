import gzip
import os

import esphome.codegen as cg
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
import esphome.config_validation as cv
from esphome.const import CONF_FILE, CONF_ID, CONF_RAW_DATA_ID
from esphome.core import HexInt

DEPENDENCIES = ["web_server"]
AUTO_LOAD = ["web_server_base"]

jk_panel_ns = cg.esphome_ns.namespace("jk_panel")
JkPanel = jk_panel_ns.class_("JkPanel", cg.Component)

# Por defecto sirve el panel.html que viene dentro del propio componente.
DEFAULT_PANEL = os.path.join(os.path.dirname(__file__), "panel.html")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JkPanel),
        cv.GenerateID(CONF_WEB_SERVER_BASE_ID): cv.use_id(web_server_base.WebServerBase),
        cv.GenerateID(CONF_RAW_DATA_ID): cv.declare_id(cg.uint8),
        cv.Optional(CONF_FILE): cv.file_,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_WEB_SERVER_BASE_ID])
    var = cg.new_Pvariable(config[CONF_ID], paren)
    await cg.register_component(var, config)

    path = str(config.get(CONF_FILE, DEFAULT_PANEL))
    with open(path, "rb") as f:
        raw = gzip.compress(f.read(), compresslevel=9, mtime=0)
    prog = cg.progmem_array(config[CONF_RAW_DATA_ID], [HexInt(b) for b in raw])
    cg.add(var.set_data(prog, len(raw)))
