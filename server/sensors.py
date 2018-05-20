from Adafruit_CCS811 import Adafruit_CCS811
from tsl2561 import TSL2561
import subprocess
from time import gmtime, strftime
import json
import time

# returns the serialized data for the cpu temp
def cpu_temp(self):
    data = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()
    return json.dumps({
        "cpu_temp": data
    })

# Returns serialized data from the tsl2561 sensor
def tsl2561(self):
    _tsl1261_client = TSL2561(debug=False)
    data = self._tsl1261_client.lux()
    return json.dumps({
        "lux": data
    })

# Returns serialized data from the ccs811 sensor
def ccs811(self):

    _ccs811_client = Adafruit_CCS811()

    data = None
    if not self._ccs811_client.readData():
        data {
            "co2": self._ccs811_client.geteCO2(),
            "tvoc": self._ccs811_client.getTVOC(),
            "temp": self._ccs811_client.calculateTemperature()
        }
    else:
        raise RuntimeError('Was not able to read air quality from the ccs811.')

    return json.dumps(data)

def init_sensors(self):
    while not self._ccs811_client.available():
        pass
    
    # Calibrate temperature
    temp = self._ccs811_client.calculateTemperature()
    self._ccs811_client.tempOffset = temp - 25.0
    self._initialized_sensors = True
