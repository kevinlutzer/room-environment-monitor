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

def parse_command_line_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description=(
                'Example Google Cloud IoT Core MQTT device connection code.'))
    parser.add_argument(
            '--project_id',
            required=True,
            help='GCP cloud project name')
    parser.add_argument(
            '--registry_id',
            required=True,
            help='Cloud IoT Core registry id')
    parser.add_argument(
            '--device_id',
            required=True,
            help='Cloud IoT Core device id')
    parser.add_argument(
            '--private_key_file',
            default="auth-files/rsa_private.pem",
            help='Path to private key file.')
    parser.add_argument(
            '--algorithm',
            default = 'RS256',
            choices=('RS256', 'ES256'),
            help='Which encryption algorithm to use to generate the JWT.')
    parser.add_argument(
            '--cloud_region',
            default='us-central1',
            help='GCP cloud region')
    parser.add_argument(
            '--ca_certs',
            default='auth-files/roots.pem',
            help=('CA root certificate from https://pki.google.com/roots.pem'))
    parser.add_argument(
            '--mqtt_bridge_hostname',
            default='mqtt.googleapis.com',
            help='MQTT bridge hostname.')
    parser.add_argument(
            '--mqtt_bridge_port',
            default=8883,
            help='MQTT bridge port.')

    return parser.parse_args()

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
    _args = None

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

    def publish_data(self, payload, args):

        # Start the network loop.
        self._client.loop_start()

        print('Publishing message {}'.format(payload))
        
        # Publish "payload" to the MQTT topic. qos=1 means 'at least one delivery'.  
        self._client.publish('/devices/{}/events'.format(args.device_id), payload, qos=1)

        # End the network loop and finish.
        self._client.loop_stop()

    @classmethod
    def create_client(cls, args):

        # Create client
        googleIotClient = GoogleIotClient()

        # Set arguements on class
        GoogleIotClient._args = args

        # Creat mqtt connection
        googleIotClient._client = mqtt.Client(client_id=('projects/{}/locations/{}/registries/{}/devices/{}'.format(args.project_id, args.cloud_region, args.registry_id, args.device_id)))
        googleIotClient._client.connect(args.mqtt_bridge_hostname, args.mqtt_bridge_port)
        
        # Username fied is not used since jwt is used crendentials
        googleIotClient._client.username_pw_set(
                username='unused',
                password=googleIotClient.create_jwt(
                        args.project_id, args.private_key_file, args.algorithm))

        # Enable SSL/TLS support.
        googleIotClient._client.tls_set(ca_certs=args.ca_certs)

        # Register message callbacks.
        googleIotClient._client.on_connect = on_connect
        googleIotClient._client.on_publish = on_publish
        googleIotClient._client.on_disconnect = on_disconnect

        return googleIotClient
