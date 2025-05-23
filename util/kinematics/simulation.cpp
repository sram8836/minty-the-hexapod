#include "InverseKinematics.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <array>
#include <chrono>
#include <thread>

const float l1 = 43.0f;  // L1: Coxa
const float l2 = 80.0f;  // L2: Femur
const float l3 = 134.0f; // L3: Tibia

// FK similar to python scripts
std::vector<std::array<float, 3>> forwardKinematics(const std::array<float, 3>& angles) {
    float theta1 = angles[0];
    float theta2 = angles[1];
    float theta3 = angles[2];

    std::vector<std::array<float, 3>> positions;

    // Base
    std::array<float, 3> base = {0.0f, 0.0f, 0.0f};
    positions.push_back(base);

    // Joint 1
    float x1 = l1 * cos(theta1);
    float y1 = l1 * sin(theta1);
    float z1 = 0.0f;
    positions.push_back({x1, y1, z1});

    // Joint 2
    float x2 = x1 + l2 * cos(theta1) * cos(theta2);
    float y2 = y1 + l2 * sin(theta1) * cos(theta2);
    float z2 = z1 + l2 * sin(theta2);
    positions.push_back({x2, y2, z2});

    // End Effector
    float x3 = x2 + l3 * cos(theta1) * cos(theta2 + theta3);
    float y3 = y2 + l3 * sin(theta1) * cos(theta2 + theta3);
    float z3 = z2 + l3 * sin(theta2 + theta3);
    positions.push_back({x3, y3, z3});

    return positions;
}

int main() {
    float radius = 200.0f;
    float height = -50.0f;
    int steps = 20;

    std::ofstream outFile("util/kinematics/leg_trajectory0.csv");
    outFile << "BaseX,BaseY,BaseZ,Joint1X,Joint1Y,Joint1Z,Joint2X,Joint2Y,Joint2Z,FootX,FootY,FootZ\n";

    for (int i = 0; i < steps; ++i) {
        float angle = 0.01f * M_PI * static_cast<float>(i) / steps;
        float x = radius * cos(angle);
        float y = radius * sin(angle-i/10);
        float z = height;

        try {
            std::array<float, 3> angles = InverseKinematics::solve(x, y, z);
            auto positions = forwardKinematics(angles);

            outFile
                << positions[0][0] << "," << positions[0][1] << "," << positions[0][2] << ","
                << positions[1][0] << "," << positions[1][1] << "," << positions[1][2] << ","
                << positions[2][0] << "," << positions[2][1] << "," << positions[2][2] << ","
                << positions[3][0] << "," << positions[3][1] << "," << positions[3][2] << "\n";

            std::cout << "Target: (" << x << ", " << y << ", " << z << ") → "
                      << "Angles [θ1=" << angles[0] << ", θ2=" << angles[1] << ", θ3=" << angles[2] << "]\n";
        } catch (const std::domain_error& e) {
            std::cerr << "Target (" << x << ", " << y << ", " << z << ") is unreachable.\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    outFile.close();
    std::cout << "Trajectory written to leg_trajectory.csv\n";
    return 0;
}
