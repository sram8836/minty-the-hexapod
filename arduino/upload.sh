#!/bin/bash

# Full path to your custom avrdude binary
AVRDUDE_BIN="/home/prithik/avrdude/build/src/avrdude"

# Arduino fqbn for your board
FQBN="arduino:megaavr:uno2018"

# Sketch directory (assumes script run from sketch directory)
SKETCH_DIR="$(pwd)"

# Output build directory
BUILD_DIR="${SKETCH_DIR}/build"

# Output hex file path
HEX_FILE="${BUILD_DIR}/arduino.ino.hex"

echo "Using avrdude binary: $AVRDUDE_BIN"
echo "Compiling sketch..."

arduino-cli compile --fqbn "$FQBN" --output-dir "$BUILD_DIR"

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

echo "Uploading sketch to device..."

sudo "$AVRDUDE_BIN" -c xplainedmini_updi -p atmega4809 -x suffer=0x7E -U flash:w:"$HEX_FILE":i

if [ $? -ne 0 ]; then
    echo "Upload failed."
    exit 1
else
    echo "Upload successful!"
fi
