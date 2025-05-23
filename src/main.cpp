#include <iostream>
#include <array>
#include <cmath>
#include "serial_writer.h"
#include "Brain.h"
#include <cmath>

int main() {

    // SerialPort serial("/dev/tty.usbmodem14201", B9600);

    // if (!serial.init()) {
    //     return 1;
    // }

    Leg leg = Leg(-M_PI/4);
    std::vector<std::tuple<float, float, float>> traj = leg.generateTrajectory(100);

    std::ofstream outFile("step_xyz.csv");
    outFile << "X, Y, Z\n";

    float x,y,z;
    for (int i=0; i < traj.size(); i++) {
        std::tie(x,y,z) = traj[i];
        outFile << x << "," << y << "," << z << std::endl;
    }

    for (int i=0; i<5; i++) {
        leg.newStep();
        
        for (int j=0; j<traj.size(); j++) {
            float a, b, c;
            std::tie(a,b,c) = traj[j];
            std::string cmd = "angles:"
                + std::to_string(a)
                + ";" + std::to_string(b)
                + ";" + std::to_string(c) + "\n";
            std::cout << cmd << std::endl;
            // serial.send(cmd);
        }
    }

    return 0;
}