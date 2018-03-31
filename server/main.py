from flask import Flask
from flask_restful import Api

import argparse

app = Flask(__name__)
api = Api(app)

def main():

    # Setup argparser
    parser = argparse.ArgumentParser()
    parser.add_argument("--stub", help="use the stub server")
    args = parser.parse_args()

    # Setup flask app
    if args.stub:
        from api_stub import GetRoomEnvironmentDataStub
        api.add_resource(GetRoomEnvironmentDataStub, '/')

    else:
        from api import GetRoomEnvironmentData, sensor_client
        sensor_client.init_sensors()
        api.add_resource(GetRoomEnvironmentData, '/')    

    app.run(debug=False)

if __name__ == '__main__':
    main()
