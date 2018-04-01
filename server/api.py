from room_monitor_data import ROOM_ENVIRONMENT_DATA_FIELDS
from flask_restful import Resource, marshal_with
from sensors import RoomMonitorSensors

sensor_client = RoomMonitorSensors()

class GetRoomEnvironmentData(Resource):

    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        a = sensor_client.collect_data()
        print a
        return a