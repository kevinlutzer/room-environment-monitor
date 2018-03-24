from flask import Flask
from flask_restful import Resource, Api, fields, marshal_with

app = Flask(__name__)
api = Api(app)

data_fields = {
    'temperature':   fields.Float,
    'humidty':    fields.Float,
    'ambientLight': fields.Float,
    'timestamp': fields.String,
    'cpuTemp': fields.Float,
    'tvoc': fields.Float,
    'co2': fields.Float,
}


class GetRoomEnvironmentData(Resource):
    @marshal_with(data_fields)
    def get(self):
        data = {
            "temperature": 25,
            "humidity": 334,
            "ambientLight": 345,
            "timestamp": "time",
            "cpuTemp": 43,
            "tvoc": 0,
            "co2": 0,
        }
        return data

api.add_resource(GetRoomEnvironmentData, '/')

if __name__ == '__main__':
    app.run(debug=True)
