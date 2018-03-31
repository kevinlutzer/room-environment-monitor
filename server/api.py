from flask import request
from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS
from flask_restful import Resource, fields, marshal_with
from sensors import RoomMonitorSensors
import json

sensor_client = RoomMonitorSensors()

class GetRoomEnvironmentData(Resource):

    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        return sensor_client.collect_data()