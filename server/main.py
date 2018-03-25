from flask import Flask
from flask_restful import Resource, Api, fields, marshal_with
import Adafruit_CCS811
import subprocess
from time import gmtime, strftime
import json

app = Flask(__name__)
api = Api(app)
css811_data = Adafruit_CCS811()

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

    def collect_data(self):

        # Get the Raspberry Pi's processor temperature. 
        if subprocess.call(["which", "/opt/vc/bin/vcgencmd"]) == 0:
            temp = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()
        # Get Mac's proc temp if "sudo gem install iStats" is installed 
        elif subprocess.call(["which", "istats"]) == 0:
            #temp = subprocess.check_output(["istats"]).decode().split(":")[-1].split("\n")[0]
            temp = "".join(subprocess.check_output(["istats"]).decode().split()).split(":")[-1].split("F")[0]
        else:
            temp = 40

        result = {
            'temperature': 0,
            "ambientLight": 4000,
            "humidity": 3000,
            "cpuTemp": css811_data.get("temp"),
            "tvoc": css811_data.get("tvoc"),
            "co2": css811_data.get("co2"),
            "timestamp": strftime("%Y-%m-%d %H:%M:%S", gmtime())
        }

        return json.dumps(result)

    def ccs811(self):

        if css811_data.available():
            temp = css811_data.calculateTemperature()
            if not css811_data.readData():
                return {
                    "co2": css811_data.geteCO2(),
                    "tvoc": css811_data.getTVOC(),
                    "temp": temp
                }

def init_sensors():
    while not css811_data.available():
        pass
    temp = css811_data.calculateTemperature()
    css811_data.tempOffset = temp - 25.0

def main():
    # Setup flask app
    api.add_resource(GetRoomEnvironmentData, '/')
    app.run(debug=True)

    init_sensors()

if __name__ == '__main__':
    main()
