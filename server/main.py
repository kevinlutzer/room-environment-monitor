from flask import Flask
from flask_restful import Api
from api import GetRoomEnvironmentData
from api_stub import GetRoomEnvironmentDataStub
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
        api.add_resource(GetRoomEnvironmentDataStub, '/')
    else:
        api.add_resource(GetRoomEnvironmentData, '/')    

    app.run(debug=False)

if __name__ == '__main__':
    main()
