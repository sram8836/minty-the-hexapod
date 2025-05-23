#include "Leg.h"
#include "InverseKinematics.h"
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

void Leg::updateState( LegState nextState, float nextCycleDuration) {
    this->nextState = nextState;
    this->nextCycleDuration = nextCycleDuration;
}

void Leg::advanceState() {
    this->currState = this->nextState;
    this->cycleDuration = this->nextCycleDuration;
}

void Leg::updateFootTrajectory( std::vector<float> stepFunction) {
    return;
}

std::array<float, 3> Leg::step(char command) {
    
    float stepLength = 60.0f;  // Step Length
    float stepHeight = 50.0f;   // Max Step Height
    const int numPoints = 100;
    float stepIncrement = 1.0f / numPoints;

    // Global Command Direction Vector (X Longitudinal, Y Sideways)
    float cmdX = 0.0f;
    float cmdY = 0.0f;

    // std::cout << "CMD: " << command << std::endl;
    switch(command) {
        case 'W':               // Foward
            cmdY = 1.0f;
            cmdX = 0.0f;
            break;
        case 'S':               // Backward
            cmdY = -1.0f;
            cmdX = 0.0f;
            break;
        case 'A':               // Left
            cmdY = 0.0f;     
            cmdX = 1.0f;
            break;
        case 'D':               // Right
            cmdY = 0.0f;
            cmdX = -1.0f;
            break;
        default:                // Stop
            cmdY = 0.0f;
            cmdX = 0.0f;
            break;
    }

    // Rotate command vector by leg base angle to get local step direction
    // x' = x*cos(θ) - y*sin(θ)
    // y' = x*sin(θ) + y*cos(θ)
    float dx_mult = cmdX * cos(baseAngle) - cmdY * sin(baseAngle);
    float dy_mult = cmdX * sin(baseAngle) + cmdY * cos(baseAngle);

    float targetX;
    float targetY;
    float targetZ;


    switch(currState) {
        case INIT: {
            stepProgress = 0.0f;
            currState = SWING;
            break;
        }

        case SWING: {
            float t = stepProgress / 0.5f;  // Maps [0, 0.5] -> [0, 1]

            float x = t * stepLength;

            float z = 4.0f * stepHeight * t * (1.0f - t);   // Vertical parabola with peak at t=0.5

            // std::cout << "x, z - " << x << " " << z << std::endl;

            float dx = x * cmdX; // Forward Component
            float dy = x * cmdY; // Lateral Component
            float dz = z;           // Vertical Component

            // std::cout << "dx, dy, dz " << dx << " " << dy << " " << dz << std::endl;

            targetX = basePosition.x + dx;
            targetY = basePosition.y + dy;
            targetZ = basePosition.z + dz;

            try {
                jointAngles = InverseKinematics::solve(targetX, targetY, targetZ);
            } catch (const std::domain_error& e) {
                std::cerr << "IK domain error: " << e.what()
                        << " at (" << targetX << ", " << targetY << ", " << targetZ << ")"
                        << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));


            stepProgress += stepIncrement;
            if (stepProgress >= 0.5f) {
                currState = SLIDE;
            }
            break;
        }

        case SLIDE: {
            float t = (stepProgress - 0.5f) / 0.5f;         // Maps [0.5, 1] -> [0,1]

            float x = (1.0f - t) * stepLength;              // Slide backward
            float z = 0.0f;                                 // No lift

            float dx = x * cmdX; // Backward Component
            float dy = x * cmdY; // Lateral Component
            float dz = z;           // Vertical Component

            targetX = basePosition.x + dx;
            targetY = basePosition.y + dy;
            targetZ = basePosition.z + dz;

            try {
                jointAngles = InverseKinematics::solve(targetX, targetY, targetZ);
            } catch (const std::domain_error& e) {
                std::cerr << "IK domain error: " << e.what()
                        << " at (" << targetX << ", " << targetY << ", " << targetZ << ")"
                        << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            stepProgress += stepIncrement;
            if (stepProgress >= 1.0f) {
                currState = FINISH;
            }
            break;

        }

        case FINISH: {
            currState = INIT;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            break;
        }
    }
    return {targetX, targetY, targetZ};
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
        currPos = step('W');
        outFile << currPos[0] << "," << currPos[1] << "," << currPos[2] << "\n";
    }

    outFile.close();
    std::cout << "Trajectory written to step_trajectory.csv\n";

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