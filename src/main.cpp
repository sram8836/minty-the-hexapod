#include <iostream>
#include <array>
#include <cmath>
#include "serial_writer.h"
#include "Brain.h"
#include <cmath>

int main() {

    Leg leg = Leg(-M_PI/4);
    std::vector<std::tuple<float, float, float>> traj = leg.generateTrajectory(100);
    // leg.takeStep();

    std::ofstream outFile("step_xyz.csv");
    outFile << "X, Y, Z\n";

    float x,y,z;
    for (int i=0; i < traj.size(); i++) {
        std::tie(x,y,z) = traj[i];
        outFile << x << "," << y << "," << z << std::endl;
    }

    outFile.close();

    return 0;
}