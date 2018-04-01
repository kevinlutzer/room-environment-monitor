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

# MQTT Callback functions for success and failure 
def error_str(rc):
    """Convert a Paho error to a human readable string."""
    return '{}: {}'.format(rc, mqtt.error_string(rc))


def on_connect(unused_client, unused_userdata, unused_flags, rc):
    """Callback for when a device connects."""
    print('Establish a MQTT Connection', error_str(rc))


def on_disconnect(unused_client, unused_userdata, rc):
    """Paho callback for when a device disconnects."""
    print('Disconnected MQTT Connection', error_str(rc))


def on_publish(unused_client, unused_userdata, unused_mid):
    """Paho callback when a message is sent to the broker."""
    print('Publish Data')

# Creates a JWT Token 
def create_jwt(project_id, private_key_file, algorithm):
    """Creates a JWT (https://jwt.io) to establish an MQTT connection.
    Args:
     project_id: The cloud project ID this device belongs to
     private_key_file: A path to a file containing either an RSA256 or ES256
         private key.
     algorithm: The encryption algorithm to use. Either 'RS256' or 'ES256'
    Returns:
      An MQTT generated from the given project_id and private key, which
      expires in 20 minutes. After 20 minutes, your client will be
      disconnected, and a new JWT will have to be generated.
    Raises:
      ValueError: If the private_key_file does not contain a known key.
    """

    token = {
        # The time that the token was issued at
        'iat': datetime.datetime.utcnow(),
        # When this token expires. The device will be disconnected after the
        # token expires, and will have to reconnect.
        'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=60),
        # The audience field should always be set to the GCP project id.
        'aud': project_id
    }

    # Read the private key file.
    with open(private_key_file, 'r') as f:
        private_key = f.read()

    return jwt.encode(token, private_key, algorithm=algorithm)

class GoogleIotClient(): 

    client = None
    args = None

    def publish_data(self, data):

        self.client.loop_start()
        mqtt_topic = '/devices/{}/events'.format(self.args.device_id)
        payload = json.dumps(data)
        self.client.publish(mqtt_topic, payload, qos=1)
        self.client.loop_stop()

    @classmethod
    def create_client(cls, args):

        # Create our MQTT client. The client_id is a unique string that identifies
        # this device. For Google Cloud IoT Core, it must be in the format below.
        client = mqtt.Client(
                client_id=(
                        'projects/{}/locations/{}/registries/{}/devices/{}'
                        .format(
                                args.project_id, args.cloud_region,
                                args.registry_id, args.device_id)))

        # With Google Cloud IoT Core, the username field is ignored, and the
        # password field is used to transmit a JWT to authorize the device.
        client.username_pw_set(
            username='unused',
            password=create_jwt(args.project_id, args.private_key_file, args.algorithm))


        # Enable SSL/TLS support.
        client.tls_set(ca_certs=args.ca_certs)

        # Register message callbacks. https://eclipse.org/paho/clients/python/docs/
        # describes additional callbacks that Paho supports. In this example, the
        # callbacks just print to standard out.
        client.on_connect = on_connect
        client.on_publish = on_publish
        client.on_disconnect = on_disconnect

        # Connect to the Google MQTT bridge.
        client.connect(args.mqtt_bridge_hostname, args.mqtt_bridge_port)

        # Create google iot client instance
        google_client = GoogleIotClient()
        google_client.client = client
        google_client.args = args
        return google_client