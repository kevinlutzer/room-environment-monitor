import datetime
import time
from time import gmtime, strftime
import subprocess
import json
# import jwt
# import paho.mqtt.client as mqtt
# from Adafruit_CCS811 import Adafruit_CCS811
# from app.command_line_args import parse_command_line_args
# from app.google_iot import GoogleIotClient

# ccs =  Adafruit_CCS811()

# def ccs811():
#     if debug:
#         return {
#             "co2": 400,
#             "tvoc": 0,
#             "temp": 25
#         }

# 	if ccs.available():
# 	    temp = ccs.calculateTemperature()
# 	    if not ccs.readData():
# 	        print "CO2: ", ccs.geteCO2(), "ppm, TVOC: ", ccs.getTVOC(), " temp: ", temp
#             return {
#                 "co2": ccs.geteCO2(),
#                 "tvoc": ccs.getTVOC(),
#                 "temp": temp
#             }
        

def collect_data():

    # Get the Raspberry Pi's processor temperature. 
    if subprocess.call(["which", "/opt/vc/bin/vcgencmd"]) == 0:
        temp = subprocess.check_output(["sudo", "/opt/vc/bin/vcgencmd", "measure_temp"]).split('=', 1)[-1].rstrip()
    # Get Mac's proc temp if "sudo gem install iStats" is installed 
    elif subprocess.call(["which", "istats"]) == 0:
        #temp = subprocess.check_output(["istats"]).decode().split(":")[-1].split("\n")[0]
        temp = "".join(subprocess.check_output(["istats"]).decode().split()).split(":")[-1].split("F")[0]
    else:
        temp = 40

    css811_data = ccs811()

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

def main():
    global debug 
    debug = True

    args = parse_command_line_args()

    while not ccs.available():
    	pass
    temp = ccs.calculateTemperature()
    ccs.tempOffset = temp - 25.0

    client = create_client(args)

    # Connect to the Google MQTT bridge.
    client.connect(args.mqtt_bridge_hostname, args.mqtt_bridge_port)

    # Collect and publish the data
    payload = collect_data()
    publish_data(client, payload, args)



    print('Finished.')


if __name__ == '__main__':
    main()
