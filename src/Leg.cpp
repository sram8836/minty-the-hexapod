#include "Leg.h"
#include "InverseKinematics.h"
#include "serial_writer.h"
#include <array>
#include <chrono>
#include <thread>
#include <fstream>

// Constructor
Leg::Leg(float aBaseAngle)
{
    currState = INIT;
    stepProgress = 0.0f;
    baseAngle = aBaseAngle;
    basePosition = {150, 0, -50};

    std::ifstream inFile("step_xyz.csv");
    if (!inFile.is_open()) {
        std::cerr << "[ERROR] Could not open step_xyz.csv\n";
        return;
    }

    std::string line;
    std::getline(inFile, line);

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string token;
        Coord coord;

        std::getline(ss, token, ',');
        coord.x = std::stof(token);

        std::getline(ss, token, ',');
        coord.y = std::stof(token);

        std::getline(ss, token, ',');
        coord.z = std::stof(token);

        stepFunction.push_back(coord);
    }

    inFile.close();
    std::cout << "Leg created at " << baseAngle << "!" << std::endl;
}

// Destructor
Leg::~Leg() {
    std::cout << "Leg Destroyed!" << std::endl;
}

std::string Leg::getState() {
    std::cout << "State:" << currState << std::endl;
    switch (currState) {
        case INIT: return "INIT";
        case SWING: return "SWING";
        case SLIDE: return "SLIDE";
        case FINISH: return "FINISH";
    }
}

float Leg::getStepProgress() {
    return stepProgress;
}

std::array<float, 3> Leg::getBasePosition() {
    return {basePosition.x, basePosition.y, basePosition.z};
}

std::array<float, 3> Leg::getJointAngles() {
    return jointAngles;
}

std::vector<std::tuple<float, float, float>> Leg::getTrajectory() {
    return trajectory;
}

void Leg::setBasePosition(const std::array<float, 3>& pos) {
    basePosition.x = pos[0];
    basePosition.y = pos[1];
    basePosition.z = pos[2];
}

void Leg::visualise() {

    
    std::ofstream outFile("step_trajectory.csv");
    if (!outFile.is_open()) {
        std::cerr << "[ERROR] Failed to open output file!" << std::endl;
    }
    outFile << "X,Y,Z\n";

    currState = INIT;
    stepProgress = 0.0f;
    basePosition = {150, 0, -50};

    std::array<float, 3> currPos;

    while (currState != FINISH) {
        // currPos = step('W');
        outFile << currPos[0] << "," << currPos[1] << "," << currPos[2] << "\n";
    }

    outFile.close();
    std::cout << "Trajectory written to step_trajectory.csv\n";

}

std::vector<std::tuple<float, float, float>> Leg::generateTrajectory(int n) {
    float x, y, z;
    float step_length = 100.0f;

    // Half Slide 1
    for (int i = 0; i <= n/4; ++i) {
        float t = static_cast<float>(i) / (n/2); // t in (-0.5, 0)
        x = x_nom - (t)*step_length*std::sin(-baseAngle);
        y = y_nom - (t)*step_length*std::cos(-baseAngle);
        z = z_nom;
        trajectory.emplace_back(x, y, z);
    }

    // Swing
    for (int i = 0; i <= n/2; ++i) {
        float t = static_cast<float>(i) / (n/2); // t in (-0.5, 0.5)
        x = x_nom + (t-0.5)*step_length*std::sin(-baseAngle);
        y = y_nom + (t-0.5)*step_length*std::cos(-baseAngle);
        z = z_nom + 4 * 50 * t * (1 - t); // Parabola
        trajectory.emplace_back(x, y, z);
    }

    // Half Slide 2
    for (int i = 0; i <= n/4; ++i) {
        float t = static_cast<float>(i) / (n/2); // t in (0, 0.5)
        x = x_nom + (0.5-t)*step_length*std::sin(-baseAngle);
        y = y_nom + (0.5-t)*step_length*std::cos(-baseAngle);
        z = z_nom;
        trajectory.emplace_back(x, y, z);
    }

    return trajectory;
}

void Leg::takeStep() {
    SerialPort serial("/dev/tty.usbmodem14201", B9600);

    if (!serial.init()) {
        return;
    }
        
    for (int j=0; j<trajectory.size(); j++) {
        float x, y,z;
        std::array<float, 3> angles;

        std::tie(x,y,z) = trajectory[j];
        angles = InverseKinematics::solve(x,y,z);

        std::string cmd = "angles:"
            + std::to_string(angles[2])
            + ";" + std::to_string(angles[1])
            + ";" + std::to_string(angles[0]) + "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        serial.send(cmd);
    }

}

void Leg::newStep() {

    currState = INIT;
    int counter = 0;

    float currX;
    float currY;
    float currZ;

    while (currState != FINISH) {
        if (counter < 50) {
            currState = SWING;
        }
        else if (counter < 100) {
            currState = SLIDE;
        }
        else {
            currState = FINISH;
        }

        currX = basePosition.x + stepFunction[counter].x;
        currY = basePosition.y + stepFunction[counter].y;
        currZ = basePosition.z + stepFunction[counter].z;

        std::array<float, 3> angles = InverseKinematics::solve(currX, currY, currZ);
        trajectory.emplace_back(angles[0], angles[1], angles[2]);
        
        counter++;
    }

}