#include "Leg.h"

// Constructor
Leg::Leg()
{
    std::cout << "Leg Created!" << std::endl;
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
    // What is this referring to?
    return 0.0;
}

std::array<float, 3> Leg::getBasePosition() {
    return basePosition;
}

void Leg::setBasePosition(const std::array<float, 3>& pos) {
    basePosition = pos;
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
    this->stepFunction = stepFunction;
}

void Leg::step(char command) {
    
    float stepLength = 100.0f;  // Step Length
    float stepHeight = 50.0f;   // Max Step Height
    const int numPoints = 100; 

    // Initialise Directional ∆s
    float dx_mult = 0.0f;
    float dy_mult = 0.0f;

    std::cout << "CMD: " << command << std::endl;
    switch(command) {
        case 'W':
            dx_mult = 1.0f;     // Foward
            dy_mult = 0.0f;
            break;
        case 'S':
            dx_mult = -1.0f;    // Backward
            dy_mult = 0.0f;
            break;
        case 'A':
            dx_mult = 0.0f;     
            dy_mult = 1.0f;     // Left
            break;
        case 'D':
            dx_mult = 0.0f;
            dy_mult = -1.0f;    // Right
            break;
        default:
            dx_mult = 0.0f;     // Stop
            dy_mult = 0.0f;
            break;
    }

    // Swing Phase
    for (size_t i=0; i < numPoints/2; ++i) {
        float t = static_cast<float>(i) / (numPoints/2 - 1);  // Normalised Progress Value

        float x;
        x = t * stepLength;

        float z = 4.0f * stepHeight * t * (1.0f - t);   // Vertical parabola with peak at t=0.5

        float dx = x * dx_mult; // Forward Component
        float dy = x * dy_mult; // Lateral Component
        float dz = z;           // Vertical Component

        float targetX = basePosition[0] + dx;
        float targetY = basePosition[1] + dy;
        float targetZ = basePosition[2] + dz;

        jointAngles = InverseKinematics::solve(targetX, targetY, targetZ);

    }

    // Slide Phase
    for (size_t i=0; i < numPoints/2; ++i) {
        float t = static_cast<float>(i) / (numPoints/2 - 1);  // Normalised Progress Value

        float x = (1.0f - t) * stepLength;              // Slide backward
        float z = 0.0f;                                 // No lift

        float dx = x * dx_mult; // Backward Component
        float dy = x * dy_mult; // Lateral Component
        float dz = z;           // Vertical Component

        float targetX = basePosition[0] + dx;
        float targetY = basePosition[1] + dy;
        float targetZ = basePosition[2] + dz;

        // jointAngles = inverseKinematics(targetX, targetY, targetZ);
    }
}

