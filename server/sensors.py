from Adafruit_CCS811 import Adafruit_CCS811
import subprocess
from time import gmtime, strftime
import json
import time

class RoomMonitorSensors(object):

    _ccs811_client = Adafruit_CCS811()
    _initialized_sensors = False

    def __init__(self, *args, **kwargs):
        self._ccs811_client = Adafruit_CCS811()


    def collect_data(self):

        ccs811_data = self.ccs811()

        if not ccs811_data:
            return ccs811_data

        cpu_temp = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()

        result = {
            "ambient_visible_light": 4000,
            "ambient_ir_light": 3405,
            "humidity": 3000,
            "cpu_temp": cpu_temp,
            "timestamp": strftime("%Y-%m-%d %H:%M:%S", gmtime())
        }.update(ccs811_data)

        return result

    def ccs811(self):

        if not self._initialized_sensors:
            self.init_sensors()

        temp = self._ccs811_client.calculateTemperature()
        if not self._ccs811_client.readData():
            return {
                "co2": self._ccs811_client.geteCO2(),
                "tvoc": self._ccs811_client.getTVOC(),
                "temp": temp
            }
        else:
            raise ValueError('Was not able to read air quality from the ccs811.')

    def init_sensors(self):
        while not self._ccs811_client.available():
            pass
        temp = self._ccs811_client.calculateTemperature()
        self._ccs811_client.tempOffset = temp - 25.0
        self._initialized_sensors = True