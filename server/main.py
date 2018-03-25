from flask import Flask
from flask_restful import Resource, Api, fields, marshal_with
from Adafruit_CCS811 import Adafruit_CCS811
import subprocess
from time import gmtime, strftime
import json

app = Flask(__name__)
api = Api(app)
css811_client = Adafruit_CCS811()

ROOM_ENVIRONMENT_DATA_FIELDS = {
    'temp':   fields.Float,
    'humidty':    fields.Float,
    'ambient_light': fields.Float,
    'timestamp': fields.String,
    'cpu_temp': fields.Float,
    'tvoc': fields.Float,
    'co2': fields.Float,
}

class GetRoomEnvironmentData(Resource):
    @marshal_with(ROOM_ENVIRONMENT_DATA_FIELDS)
    def get(self):
        return self.collect_data()

    def collect_data(self):

        ccs811_data = self.ccs811()

        # Get the Raspberry Pi's processor temperature. 
        if subprocess.call(["which", "/opt/vc/bin/vcgencmd"]) == 0:
            cpu_temp = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()
        # Get Mac's proc temp if "sudo gem install iStats" is installed 
        elif subprocess.call(["which", "istats"]) == 0:
            #temp = subprocess.check_output(["istats"]).decode().split(":")[-1].split("\n")[0]
            cpu_temp = "".join(subprocess.check_output(["istats"]).decode().split()).split(":")[-1].split("F")[0]
        else:
            cpu_temp = 40

        result = {
            "ambient_light": 4000,
            "humidity": 3000,
            "temp": ccs811_data.get("temp"),
            "cpu_temp": cpu_temp,
            "tvoc": ccs811_data.get("tvoc"),
            "co2": ccs811_data.get("co2"),
            "timestamp": strftime("%Y-%m-%d %H:%M:%S", gmtime())
        }

        return json.dumps(result)

    def ccs811(self):

        if css811_client.available():
            temp = css811_client.calculateTemperature()
            if not css811_client.readData():
                return {
                    "co2": css811_client.geteCO2(),
                    "tvoc": css811_client.getTVOC(),
                    "temp": temp
                }

def init_sensors():
    while not css811_client.available():
        pass
    temp = css811_client.calculateTemperature()
    css811_client.tempOffset = temp - 25.0

def main():
    # Setup flask app
    api.add_resource(GetRoomEnvironmentData, '/')
    app.run(debug=True)

    init_sensors()

if __name__ == '__main__':
    main()
