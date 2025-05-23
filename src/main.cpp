#include <iostream>
#include <array>
#include <cmath>
#include "serial_writer.h"
#include "Brain.h"
#include "Leg.h"
#include "InverseKinematics.h"

#include <chrono>
#include <thread>

int main() {

    // Brain hexapod;

    // std::cout << "Hexapod" << std::endl;

    // hexapod.inputGait();
    // hexapod.update();

    SerialPort serial("/dev/tty.usbmodem14201", B9600);

    if (!serial.init()) {
        return 1;
    }

    Leg leg = Leg(-M_PI/4);
    for (int i=0; i<200; i++) {
        leg.step('W');
        std::array<float, 3> angles = leg.getJointAngles();
        std::string cmd = "angles:"
            + std::to_string(angles[2])
            + ";" + std::to_string(angles[1])
            + ";" + std::to_string(angles[0]) + "\n";
        serial.send(cmd);
    }

    // int forward = 1;
    // float min = -120;
    // float max = 120;
    // float y = min;

    // while (1) {
        
    //     if (forward && y < max) {
    //         y += 2.0;
    //     }
    //     else if (!forward && y > min) {
    //         y -= 2.0;
    //     }
    //     else {
    //         forward = !forward;
    //     }

    //     leg.test_step(y);
    //     std::array<float, 3> angles = leg.getJointAngles();

    //     std::string cmd = "angles:"
    //         + std::to_string(angles[2])
    //         + ";" + std::to_string(angles[1])
    //         + ";" + std::to_string(angles[0]) + "\n";
    //     serial.send(cmd);

    //     std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // }

    // std::array<float, 3> jointAngles = InverseKinematics::solve(150.0, 75.0, 0.0);
    // std::string cmd = "angles:"
    //     + std::to_string(jointAngles[2])
    //     + ";" + std::to_string(jointAngles[1])
    //     + ";" + std::to_string(jointAngles[0]) + "\n";
    // serial.send(cmd);

    return 0;
}