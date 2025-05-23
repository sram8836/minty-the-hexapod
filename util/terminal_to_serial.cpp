#include "../include/serial_writer.h"

int main() {

    SerialPort serial("/dev/tty.usbmodem14301", B9600);

    if (!serial.init()) {
        return 1;
    }

    std::string line;
    std::cout << "Enter text to send over serial (Ctrl+D to exit):" << std::endl;

    while (std::getline(std::cin, line)) {
        serial.send(line + "\n");
    }

    return 0;
}
