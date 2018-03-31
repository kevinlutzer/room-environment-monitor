import datetime
import time
from time import gmtime, strftime
import subprocess
import json
import jwt
import paho.mqtt.client as mqtt
from google_iot import GoogleIotClient
from urlfetch import get, UrlfetchException

LOCAL_SERVER_HOST = 'localhost'
LOCAL_SERVER_PORT = '5000'

PROJECT_CONFIG = {
    'project_id': 'personal-website-klutzer',
    'cloud_region': 'us-central1',
    'registry_id': 'klutzer-devices',
    'device_id': 'raspberry-pi-room-monitor-rs256-device',
    'private_key_file': 'rsa_private.pem',
    'algorithm': 'RS256',
    'ca_certs': 'roots.pem',
    'mqtt_bridge_hostname': 'mqtt.googleapis.com',
    'mqtt_bridge_port': 8883,
}

def main():

    iot_client = GoogleIotClient.create_client(PROJECT_CONFIG)
    data = None

    try: 
        response = get(LOCAL_SERVER_HOST + ':' + LOCAL_SERVER_PORT)
        if response.status_code != 200: 
            raise ValueError('Was not able to fetch the data correctly')
        data = json.load(response.content)
    except UrlfetchException:
        return

    # Collect and publish the data
    iot_client.publish_data(data)


if __name__ == '__main__':
    main()
