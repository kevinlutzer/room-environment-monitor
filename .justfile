# .justfile

# Set the shell for running recipes
set shell := ["bash", "-cu"]

# Lint recipe using cargo clippy
lint:
    #!/bin/bash

    find . -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

# Setup Arduino-CLI, libs, and platformio as well as all needed Arduino-CLI libs
bootstrap:
    #!/bin/bash
    LIBS=("Adafruit BME280" "Adafruit NeoPixel" "PM1006K" "PubSubClient" "UUID" "Adafruit SGP40 Sensor" "ArduinoJson" "Adafruit BME280 Library")

    # Verify current user is root
    if [[ $EUID -ne 0 ]]; then
        echo "This script must be run as root" 1>&2
        exit 1
    fi

    # Verify arduino cli installer
    echo "Checking if Arduino CLI is installed..."
    if [[ "$(which arduino-cli)" == "" ]]; then
        echo "Arduino CLI doesn't exist, installing now to /usr/local/bin"
        curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh
    fi

    echo "Arduino CLI is installed"

    # Install platform io cli
    echo "Checking if Platform IO is installed..."
    if [[ "$(which platformio)" == "" ]]; then 
        echo "Platform IO not installed, installing now ..."
        curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
        python3 get-platformio.py
    fi 
    echo "Platform IO is installed"

    # Install arduino libs
    echo "Install Arduino CLI libs..."
    for key in "${LIBS[@]}"; do
        arduino-cli lib install "$key"
    done

# Build the Docker image used for CI-CD
build-cicd-image:
    #!/bin/bash
    docker build -t kevinlutzer/rem-iot-cicd:latest -f docker/Dockerfile.cicd .

# Push CI-CD Docker image to Docker Hub
push-cicd-image:
    #!/bin/bash
    docker push kevinlutzer/rem-iot-cicd:latest