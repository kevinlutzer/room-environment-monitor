from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS
from flask_restful import Resource, marshal_with
from sensors import RoomMonitorSensors

sensor_client = RoomMonitorSensors()

class GetRoomEnvironmentData(Resource):
    """ Get Room Environment Data Flask Resource"""
    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        """ get handler """
        return sensor_client.collect_data()