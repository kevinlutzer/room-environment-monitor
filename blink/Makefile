BOARD?=esp32:esp32:esp32c3
PORT?=/dev/cu.usbserial-210
BUILD=build

.PHONY: default lint all flash clean

default: lint all flash clean

# lint:
# 	cpplint --extensions=ino --filter=-legal/copyright *.ino

all:
	arduino-cli compile --fqbn $(BOARD) --output-dir $(BUILD) -v ./

flash:
	arduino-cli upload --fqbn $(BOARD) --port $(PORT) --input-dir $(BUILD)

clean:
	rm -r build