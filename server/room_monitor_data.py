from flask_restful import fields

ROOM_ENVIRONMENT_DATA_FIELDS = {
    "lux": fields.Integer,
    "temp": fields.String,
    "cpu_temp": fields.String,
    "tvoc": fields.Integer,
    "co2": fields.Float,
    "timestamp": fields.String
}