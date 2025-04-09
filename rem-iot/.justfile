# .justfile

# Set the shell for running recipes
set shell := ["bash", "-cu"]

# Lint recipe using cargo clippy
lint:
    find . -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

boostrap:
    #!/bin/bash
    LIBS=("Adafruit BME280" "Adafruit NeoPixel" "Arduino PM1006K" "PubSubClient" "UUID" "Adafruit_SGP40")

    # Verify arduino cli installer
    echo "Checking if Arduino CLI is installed..."
    if [[ "$(which arduino-cli)" == "" ]]; then
        echo "Arduino CLI doesn't exist, installing now to /usr/local/bin"
        curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sudo BINDIR=/usr/local/bin sh
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
        arduino-cli lib install $key
    done
