
from Adafruit_CCS811 import Adafruit_CCS811
from tsl2561 import TSL2561
import subprocess
from time import gmtime, strftime
import json
import time
import argparse

def parse_command_line_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--sensor",
        default="gas",
        help="The sensor to get data for")
    parser.add_argument(
        "--stub",
        default=False,
        help="The sensor to get data for")
    return parser.parse_args()

# Returns serialized data from the tsl2561 sensor
def tsl2561():
    tsl1261_client = TSL2561(debug=False)
    data = tsl1261_client.lux()
    return json.dumps({
        "lux": data
    })

# Returns serialized data from the ccs811 sensor
def ccs811():
    ccs811_client = Adafruit_CCS811()

    data = None
    if not ccs811_client.readData():
        data = {
            "co2": ccs811_client.geteCO2(),
            "tvoc": ccs811_client.getTVOC(),
            "temp": ccs811_client.calculateTemperature()
        }
    else:
        raise RuntimeError('Was not able to read air quality from the ccs811.')

    return json.dumps(data)

def init_ccs811():
    ccs811_client = Adafruit_CCS811()

    while not ccs811_client.available():
        pass
    
    # Calibrate temperature
    temp = ccs811_client.calculateTemperature()
    ccs811_client.tempOffset = temp - 25.0

    print "success"

# def evaluateStub(sensor):
#     if sensor == "light":
#         print "{\"lux\": 4000}"
#     elif sensor == "initialize_light":
#         print "success"
#     elif sensor == "gas":
#         print "{\"temp\": 43.2, \"tvoc\": 4000, \"co2\": 4000.5}"
#     else:
#         raise RuntimeError("Not a valid sensor")

def evaluateSensors(sensor):
    if sensor == "light":
        print tsl2561()
    elif sensor == "initialize_light":
        init_ccs811()
        print "success"
    elif sensor == "gas":
        print ccs811()
    else: 
        raise RuntimeError("Not a valid sensor")

def main():

    args = parse_command_line_args()

    val = args.sensor
    evaluateSensors(val)
    
if __name__ == '__main__':
    main()
