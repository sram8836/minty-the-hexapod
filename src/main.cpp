#include <iostream>
#include <array>
#include "serial_writer.h"
#include "Brain.h"
#include "Leg.h"
#include "InverseKinematics.h"

int main() {

    // Brain hexapod;

    // std::cout << "Hexapod" << std::endl;

    // hexapod.inputGait();
    // hexapod.update();

    SerialPort serial("/dev/tty.usbmodem14301", B9600);

    if (!serial.init()) {
        return 1;
    }

    std::array<float, 3> jointAngles = InverseKinematics::solve(150.0, 0.0, -50.0);
    std::string cmd = "angles:"
        + std::to_string(jointAngles[2])
        + ";" + std::to_string(jointAngles[1])
        + ";" + std::to_string(jointAngles[0]) + "\n";

    serial.send(cmd);
    return 0;
}