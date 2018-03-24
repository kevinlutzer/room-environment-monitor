import argparse
import datetime
import time
from time import gmtime, strftime
import subprocess

import json
import jwt
import paho.mqtt.client as mqtt
from urlfetch import get, UrlfetchException


# MQTT Callback functions for success and failure
def error_str(rc):
    """Convert a Paho error to a human readable string."""
    return '{}: {}'.format(rc, mqtt.error_string(rc))


def on_connect(unused_client, unused_userdata, unused_flags, rc):
    """Callback for when a device connects."""
    print('on_connect', error_str(rc))


def on_disconnect(unused_client, unused_userdata, rc):
    """Paho callback for when a device disconnects."""
    print('on_disconnect', error_str(rc))


def on_publish(unused_client, unused_userdata, unused_mid):
    """Paho callback when a message is sent to the broker."""
    print('on_publish')

class GoogleIotClient(): 

    _client = None

    @classmethod
    def create_jwt(self, project_id, private_key_file, algorithm):
        """Create JWT, will throw ValueError if private key file doesn't exist or is invalid"""

        token = {
            # The time that the token was issued at
            'iat': datetime.datetime.utcnow(),
            # Expiry of the token
            'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=60),
            # The audience field should always be set to the GCP project id.
            'aud': project_id
        }

        # Read the private key file.
        with open(private_key_file, 'r') as f:
            private_key = f.read()

        print(
                'Creating JWT using {} from private key file {}'.format(
                        algorithm, private_key_file))

        return jwt.encode(token, private_key, algorithm=algorithm)

    def publish_data(self, client, payload, args):

        # Start the network loop.
        client.loop_start()

        print('Publishing message {}'.format(payload))
        
        # Publish "payload" to the MQTT topic. qos=1 means 'at least one delivery'.  
        client.publish('/devices/{}/events'.format(args.device_id), payload, qos=1)

        # End the network loop and finish.
        client.loop_stop()

    @classmethod
    def create_client(cls, args):
        # Create client
        googleIotClient = GoogleIotClient()
        googleIotClient._client = mqtt.Client(client_id=('projects/{}/locations/{}/registries/{}/devices/{}'.format(args.project_id, args.cloud_region, args.registry_id, args.device_id)))

        # Username fied is not used since jwt is used crendentials
        googleIotClient._client.username_pw_set(
                username='unused',
                password=GoogleIotClient.create_jwt(
                        args.project_id, args.private_key_file, args.algorithm))

        # Enable SSL/TLS support.
        googleIotClient._client.tls_set(ca_certs=args.ca_certs)

        # Register message callbacks.
        googleIotClient._client.on_connect = on_connect
        googleIotClient._client.on_publish = on_publish
        googleIotClient._client.on_disconnect = on_disconnect

        return googleIotClient

    @classmethod
    def fetch_data(cls):
        try: 
            response = get('localhost:5000')
            
            if response.status != 200:
                raise ValueError('Reponse from api fetch was not a 200')
            
            return response.content

        except UrlfetchException: 
            raise         
