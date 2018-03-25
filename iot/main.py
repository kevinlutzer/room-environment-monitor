import datetime
import time
from time import gmtime, strftime
import subprocess
import json
import jwt
import paho.mqtt.client as mqtt
from .google_iot import GoogleIotClient, parse_command_line_args
from urlfetch import get, UrlfetchException

HOST = 'localhost:5000'

def main():

    args = parse_command_line_args()
    iot_client = GoogleIotClient.create_client(args)
    content = None

    try: 
        response = get(HOST)
        if response.status_code != 200: 
            raise ValueError('Was not able to fetch the data correctly')
        content = response.content
    except UrlfetchException:
        return 

    # Collect and publish the data
    iot_client.publish_data(content, args)


if __name__ == '__main__':
    main()
