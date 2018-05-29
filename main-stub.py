import argparse

def parse_command_line_args():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--sensor",
        default="gas",
        help="The sensor to get data for")
    parser.add_argument(
        "--stub",
        default=False,
        help="The sensor to get data for")
    return parser.parse_args()

def evaluateStub(sensor):
    if sensor == "light":
        print "{\"lux\": 4000}"
    elif sensor == "initialize_light":
        print "success"
    elif sensor == "gas":
        print "{\"temp\": 43.2, \"tvoc\": 4000, \"co2\": 4000.5}"
    else:
        raise RuntimeError("Not a valid sensor")

def main():

    args = parse_command_line_args()

    val = args.sensor
    evaluateStub(val)
    
if __name__ == '__main__':
    main()