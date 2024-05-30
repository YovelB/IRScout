# Target Guide Controller

This is a project uses an Arduino to create a modulated signal and sending it to IR Transmitter.

## Requirements

- Any Arduino board with minimum two hardware timers.
- IR led connected to pin 11.
- Any supported IDE(Arduino or PlatformIO)

## Build

1. Clone this repository to your local machine using `git clone <repository-url>`.
2. Open the project in your chosen IDE (PlatformIO or Arduino IDE).
3. Connect your Arduino board to your computer using a USB cable.
4. In your IDE, select the correct board model and port:
    - In PlatformIO, you can set the board model in the `platformio.ini` file and the port will be automatically
      detected.
    - In Arduino IDE, you can select the board model from the `Tools > Board` menu and the port from the `Tools > Port`
      menu.
5. Build the project:
    - In PlatformIO, click on the "Build" button in the toolbar or use the `PlatformIO: Build` command from the command
      palette.
    - In Arduino IDE, select `Sketch > Verify/Compile` from the menu.
6. Upload the project to your Arduino board:
    - In PlatformIO, click on the "Upload" button in the toolbar or use the `PlatformIO: Upload` command from the
      command palette.
    - In Arduino IDE, select `Sketch > Upload` from the menu.

## Usage

Once the project is uploaded to your Arduino board, the IR LED will start transmitting an IR signal. The signal's
frequency and duty cycle are determined by the `MODULATED_FREQUENCY` and `DUTY_CYCLE` parameters in the `IRController`
class.

You can modify these parameters in the `Main.cpp` file and in the `IRController::getInstance()` call in your code to
change the frequency and duty cycle of the IR signal.

### Warning

Be careful when setting the `DUTY_CYCLE` parameter. A higher duty cycle means the IR LED will be on for a larger
proportion of the time. If the duty cycle is too high, the IR LED might overheat and burn out. Always ensure that your
duty cycle is appropriate for your specific IR LED and that you're not exceeding its maximum power rating.

## Testing

You can run the tests for this project only in PlatformIO by following these steps:

1. Make sure to follow the build instructions above.
2. Run the tests:
    - In Clion, open the terminal and run the command `pio test -e 'env'`.
    - In Visual Studio Code, there are two ways to run the tests:
        - Use the PlatformIO toolbar on `advanced->Test` and select the environment.
        - Open the terminal and run the command `pio test -e 'env'`.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.