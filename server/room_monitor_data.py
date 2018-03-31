from flask import request
from flask_restful import Resource, fields, marshal_with
import json

ROOM_ENVIRONMENT_DATA_FIELDS = {
    "ambient_visible_light": fields.Float,
    "ambient_ir_light": fields.Float,
    "humidity": fields.Float,
    "temp": fields.Float,
    "cpu_temp": fields.Float,
    "tvoc": fields.Integer,
    "co2": fields.Float,
    "timestamp": fields.String
}