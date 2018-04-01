from flask import Flask
from flask_restful import Api

import argparse

app = Flask(__name__)
api = Api(app)

def parse_command_line_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--stub",
        default=False,
        help="use the stub server")
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
    """Entry point in app."""
    # Setup argparser
    args = parse_command_line_args()

    # Setup flask app, use stub if the required argument is passed
    if args.stub:
        from api_stub import GetRoomEnvironmentDataStub
        api.add_resource(GetRoomEnvironmentDataStub, '/')

    else:
        print 'Not using stub'
        from api import GetRoomEnvironmentData, sensor_client
        sensor_client.init_sensors()
        print sensor_client.collect_data()
        api.add_resource(GetRoomEnvironmentData, '/')    

    app.run(
        debug=False,
        port=args.local_server_port,
        host=args.local_server_host
    )

if __name__ == '__main__':
    main()
