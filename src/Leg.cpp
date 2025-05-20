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

void Leg::getState() {
    std::cout << "State:" << currState << std::endl;
}

float Leg::getStepProgress() {
    // What is this referring to?
    return 0.0;
}

void Leg::updateState( LegState nextState, float nextCycleDuration) {
    this->currState = this->nextState;
    this->nextState = nextState;
    this->cycleDuration = this->nextCycleDuration;
    this->nextCycleDuration = nextCycleDuration;
}

void Leg::updateFootTrajectory( std::vector<float> stepFunction) {
    this->stepFunction = stepFunction;
}

void Leg::step(double dir_angle) {
    
    float stepLength = 100.0f;  // Step Length
    float stepHeight = 50.0f;   // Max Step Height
    const int numPoints = 100; 

    for (size_t i=0; i < numPoints; ++i) {
        float t = static_cast<float>(i) / (numPoints - 1);  // Normalised Progress Value

        float x;
        if (t < 0.5f) {
            x = 2 * t * stepLength;
        }
        else {
            x = 2 * (1.0f - t) * stepLength;
        }

        float z = 4.0f * stepHeight * t * (1.0f - t);   // Parabola with peak at t=0.5

        float dx = x * std::cos(dir_angle); // Forward Component
        float dz = z;                       // Vertical Component
        // No y because dir_angle changes XZ plane that step trajectory is on

        // jointAngles = inverseKinematics(dx, dz);

    }
}

