# SM-21 Relay System Control

This Arduino code is designed to control a relay system for the SM-23, providing functionalities for ignition, charging, operation, and discharge states of the bike and transitions between them. 

## Table of Contents

- [Requirements](#requirements)
- [Setup](#setup)
- [Usage](#usage)
- [Functions](#functions)

## Requirements

To run this code, you'll need the following:

- **Arduino board:** The microcontroller responsible for running the code.
- **SM-23 system with relay connections:** The target system with relay modules connected to control various functions.
- **Charge button:** An input button to initiate the charging process.
- **Ignition keys:** Input keys to control the ignition process.
- **Directional inputs (Forward and Reverse):** Switches or buttons indicating the direction of movement.

## Setup

1. **Connect the relay system:** Attach the relay system to the corresponding pins on your Arduino board.
2. **Connect the charge button:** Attach the charge button to the designated pin.
3. **Connect the ignition keys:** Attach the ignition keys to the specified pin.
4. **Connect directional inputs:** Attach switches or buttons for forward and reverse to the respective pins.
5. **Upload the Arduino code:** Use the Arduino IDE to upload the provided code to your Arduino board.

## Usage

1. **Check connections:** Ensure all connections are set up correctly.
2. **Power on the Arduino board:** Provide power to the Arduino board.
3. **Follow specified inputs:**
    - Press the charge button to initiate charging.
    - Turn the ignition keys to start the ignition process.
    - Use directional inputs to control the forward and reverse movements.
4. **Monitor the Arduino Serial Monitor:** Check for status updates and diagnostic messages.

## Functions

### `allRelaysOpen()`

Opens all relays to avoid conflicts when switching between states.

### `allDigitalRead()`

Reads digital inputs for charge, ignition, and directional signals.

### `park(int forward_input, int reverse_input)`

Determines if the bike is in park based on the state of directional inputs.

