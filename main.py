
# from flask import Flask
# from flask_restful import Api


# app = Flask(__name__)
# api = Api(app)

# from Adafruit_CCS811 import Adafruit_CCS811
# from tsl2561 import TSL2561
# import subprocess
# from time import gmtime, strftime
# import json
# import time
# import argparse

# def parse_command_line_args():
#     """Parse command line arguments."""
#     parser = argparse.ArgumentParser()
#     parser.add_argument(
#         "--sensor",
#         default=False,
#         help="The sensor to get data for")
#     return parser.parse_args()


# # returns the serialized data for the cpu temp
# def cpu_temp():
#     data = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()
#     return json.dumps({
#         "cpu_temp": data
#     })

# # Returns serialized data from the tsl2561 sensor
# def tsl2561():
#     tsl1261_client = TSL2561(debug=False)
#     data = tsl1261_client.lux()
#     return json.dumps({
#         "lux": data
#     })

# # Returns serialized data from the ccs811 sensor
# def ccs811():
#     ccs811_client = Adafruit_CCS811()

#     data = None
#     if not ccs811_client.readData():
#         data = {
#             "co2": ccs811_client.geteCO2(),
#             "tvoc": ccs811_client.getTVOC(),
#             "temp": ccs811_client.calculateTemperature()
#         }
#     else:
#         raise RuntimeError('Was not able to read air quality from the ccs811.')

#     return json.dumps(data)

# def init_ccs811():
#     ccs811_client = Adafruit_CCS811()

#     while not ccs811_client.available():
#         pass
    
#     # Calibrate temperature
#     temp = ccs811_client.calculateTemperature()
#     ccs811_client.tempOffset = temp - 25.0


def main():

    # args = parse_command_line_args()
    
    # val = args.sensor
    # if val == "temp":
    #     print cpu_temp()
    #     return
    # elif val == "gas":
    #     print tsl2561()
    #     return
    # elif val == "light":
    #     init_ccs811()
    #     print ccs811()
    #     return
    # else: 
    #     raise RuntimeError("Not a valid sensor")
    print "{\"cpu_temp\": \"246.2 C\"}"
        
if __name__ == '__main__':
    main()
