from flask import request
from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS
from flask_restful import Resource, fields, marshal_with
from main import sensors
import json


class GetRoomEnvironmentData(Resource):

    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        return sensors.collect_data()