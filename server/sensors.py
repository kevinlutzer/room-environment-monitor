from Adafruit_CCS811 import Adafruit_CCS811
import subprocess
from time import gmtime, strftime
import json

class RoomMonitorSensors(object):

    _ccs811_client = Adafruit_CCS811()

    def __init__(self, *args, **kwargs):
        self._ccs811_client = Adafruit_CCS811()


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
            "ambient_visible_light": 4000,
            "ambient_ir_light": 3405,
            "humidity": 3000,
            "temp": ccs811_data.get("temp"),
            "cpu_temp": cpu_temp,
            "tvoc": ccs811_data.get("tvoc"),
            "co2": ccs811_data.get("co2"),
            "timestamp": strftime("%Y-%m-%d %H:%M:%S", gmtime())
        }
        return result

    def ccs811(self):

        if self._ccs811_client.available():
            temp = self._ccs811_client.calculateTemperature()
            if not self._ccs811_client.readData():
                return {
                    "co2": self._ccs811_client.geteCO2(),
                    "tvoc": self._ccs811_client.getTVOC(),
                    "temp": temp
                }

    def init_sensors(self):
        while not self._ccs811_client.available():
            pass
        temp = self._ccs811_client.calculateTemperature()
        self._ccs811_client.tempOffset = temp - 25.0
