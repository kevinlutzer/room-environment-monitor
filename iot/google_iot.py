import argparse
import datetime
import time
from time import gmtime, strftime
import subprocess

import json
import jwt
import paho.mqtt.client as mqtt
from urlfetch import get, UrlfetchException
import argparse

"""
args definition

    'project_id': 'personal-website-klutzer',
    'cloud_region': 'us-central1',
    'registry_id': 'klutzer-devices',
    'device_id': 'raspberry-pi-room-monitor-rs256-device',
    'private_key_file': 'rsa_private.pem',
    'algorithm': 'RS256',
    'ca_certs': 'roots.pem',
    'mqtt_bridge_hostname': 'mqtt.googleapis.com',
    'mqtt_bridge_port': 8883,

"""

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

    client = None
    args = None

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

    def publish_data(self, payload):

        # Start the network loop.
        self._client.loop_start()

        print('Publishing message {}'.format(payload))
        
        # Publish "payload" to the MQTT topic. qos=1 means 'at least one delivery'.  
        self._client.publish('/devices/{}/events'.format(self._args.device_id), payload, qos=1)

        # End the network loop and finish.
        self._client.loop_stop()
    
    @classmethod
    def verify_args(cls, args):
        """ Raises a value error if required params are missing """
        if not args.get('project_id'):
            ValueError('project_id is required')
        if not args.get('cloud_region'):
            ValueError('project_id is required')
        if not args.get('device_id'):
            ValueError('device_id is required')
        if not args.get('private_key_file'):
            ValueError('private_key_file is required')
        if not args.get('algorithm'):
            ValueError('algorithm is required')
        if not args.get('ca_certs'):
            ValueError('ca_certs is required')
        if not args.get('mqtt_bridge_hostname'):
            ValueError('mqtt_bridge_hostname is required')
        if not args.get('mqtt_bridge_port'):
            ValueError('mqtt_bridge_port is required')

    @classmethod
    def create_client(cls, args):

        # Create client
        client = GoogleIotClient()

        # Set arguements on class
        cls.verify_args(args)
        client.args = args

        # Creat mqtt connection
        client.client = mqtt.Client(client_id=('projects/{}/locations/{}/registries/{}/devices/{}'.format(client.args.get('project_id'), 
        client.args.get('cloud_region'), client.args.get('registry_id'), client.args.get('device_id')))
        client.client.connect(client.args.get('mqtt_bridge_hostname'), client.args.get('mqtt_bridge_port'))
        
        # Username fied is not used since jwt is used crendentials
        client.client.username_pw_set(
                username='unused',
                password=client.create_jwt(
                        client._args.get('project_id'), client.args.get('private_key_file'), client.args.get('algorithm'))

        # Enable SSL/TLS support.
        client.client.tls_set(ca_certs=args.get('ca_certs'))

        # Register message callbacks.
        client.client.on_connect = on_connect
        client.client.on_publish = on_publish
        client.client.on_disconnect = on_disconnect

        return client