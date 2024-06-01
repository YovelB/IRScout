# IRScout

This project is a cart that moves towards the IR receiver and avoids obstacles.

## Requirements

- Any Arduino board with at least few digital pins.
- Motor shield to control the motors.
- IR receiver like TSOP3838.
- Ultrasonic sensor like HC-SR04.
- (Optional) Bluetooth module like HC-05.
- 9V battery to power the motors and Arduino.
- Any supporting hardware like motors, wheels, chassis, etc.
- Arduino IDE or PlatformIO.

## Build

1. Clone this repo branch using `gh repo clone <repo> -- --branch <branch>`.
2. Open the project in Arduino IDE or PlatformIO.
3. Build the cart using the hardware mentioned in the requirements.
4. In your IDE, select the correct board and port:
    - In PlatformIO, edit `platformio.ini` and set the correct board and port.
    - In Arduino IDE, select the correct board and port from the Tools menu.
5. Build the project:
    - In PlatformIO, click on the "Build" button in the toolbar or use the `PlatformIO: Build` command from the command
      palette.
    - In Arduino IDE, select `Sketch > Verify/Compile` from the menu.
6. Upload the project to your Arduino board:
    - In PlatformIO, click on the "Upload" button in the toolbar or use the `PlatformIO: Upload` command from the
      command palette.
    - In Arduino IDE, select `Sketch > Upload` from the menu.

## Usage

1. Power the Arduino and the motors using the 9V battery.
2. You can start and stop the cart using the bluetooth module by sending the commands `ON` and `OF` respectively.
3. The cart will move towards the IR receiver and avoid obstacles according to the state machine implemented in the
   GuidedCart class.
4. You can change a few parameters in the GuidedCart header file to adjust the cart's behavior.

## Known Issues
1. The IR receiver can receive reflections from the walls and other objects, which can cause the cart to move in the
   wrong direction.
2. Ultrasonic sensors can sometimes give false readings, which can cause the cart to stop unnecessarily.
3. Cart can get stuck in a loop if it cannot find a clear path to the IR receiver.
4. The search IR state can sometimes cause rotation that does not return to the original position.

## Testing

You can run the tests using the PlatformIO test framework. To run the tests, follow these steps:

1. Make sure to follow the build steps mentioned above.
2. Run the tests:
    - In Clion, open the terminal and run the command `pio test -e 'env'`.
    - In Visual Studio Code, there are two ways to run the tests:
        - Use the PlatformIO toolbar on `advanced->Test` and select the environment.
        - Open the terminal and run the command `pio test -e 'env'`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.