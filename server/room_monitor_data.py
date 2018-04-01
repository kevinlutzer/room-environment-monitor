from flask_restful import fields

ROOM_ENVIRONMENT_DATA_FIELDS = {
    "ambient_visible_light": fields.Float,
    "ambient_ir_light": fields.Float,
    "humidity": fields.Float,
    "temp": fields.String,
    "cpu_temp": fields.String,
    "tvoc": fields.Integer,
    "co2": fields.Float,
    "timestamp": fields.String
}