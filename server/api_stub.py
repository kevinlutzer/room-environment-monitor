from flask_restful import Resource, marshal_with
from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS

class GetRoomEnvironmentDataStub(Resource):
    """ Get Room Environment Data Flask Resource"""
    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        """ get handler """
        return {
            "lux": 41,
            "temp": 34,
            "cpu_temp": 34,
            "tvoc": 1,
            "co2": 45,
            "timestamp": "2018-03-31 20:41:40"
        }