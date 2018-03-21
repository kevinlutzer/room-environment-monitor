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