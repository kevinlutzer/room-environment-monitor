from flask import request
from flask_restful import Resource, fields, marshal_with
from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS
import json


class GetRoomEnvironmentData(Resource):

    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        return {
            "ambient_visible_light": 4000,
            "ambient_ir_light": 3405,
            "humidity": 3000,
            "temp": 34,
            "cpu_temp": 34,
            "tvoc": 1,
            "co2": 45,
            "timestamp": "2018-03-31 20:41:40"
        }