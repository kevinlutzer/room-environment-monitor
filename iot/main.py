import json
from google_iot import GoogleIotClient
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
            required=True,
            help='Path to private key file.')
    parser.add_argument(
            '--algorithm',
            default='RS256',
            required=True,
            help='Which encryption algorithm to use to generate the JWT.')
    parser.add_argument(
            '--cloud_region',
            default='us-central1',
            help='GCP cloud region')
    parser.add_argument(
            '--ca_certs',
            default='roots.pem',
            help=('CA root certificate from https://pki.google.com/roots.pem'))
    parser.add_argument(
            '--mqtt_bridge_hostname',
            default='mqtt.googleapis.com',
            help='MQTT bridge hostname.')
    parser.add_argument(
            '--mqtt_bridge_port',
            default=8883,
            help='MQTT bridge port.')
    parser.add_argument(
            '--local_server_host',
            default='localhost',
            help='Local server host name')
    parser.add_argument(
            '--local_server_port',
            default=5000,
            help='Local server port')

    return parser.parse_args()


def main():

    args = parse_command_line_args()
    iot_client = GoogleIotClient.create_client(args)
    
    data = None
    try: 
        response = get(args.local_server_host + ':' + args.local_server_port)
        if response.status_code != 200: 
            raise ValueError('Was not able to fetch the data correctly')
        data = json.loads(response.content)
    except UrlfetchException:
        return

    #Collect and publish the data
    iot_client.publish_data(data)

 
if __name__ == '__main__':
    main()
