#include "Leg.h"
#include <iostream>

int main() {
    // Set a single target point
    float x = 150.0f;
    float y = 100.0f;
    float z = -50.0f;

    // Dummy state manager
    StateTransmitter dummyStateManager;

    // Create a leg with arbitrary index and mountAngle (not important for direct testing)
    Leg leg(0, 0.0f, &dummyStateManager);

    try {
        std::vector<float> angles = leg.solveInverseKinematics(x, y, z);

        std::cout << "Target: (" << x << ", " << y << ", " << z << ") → "
                  << "Angles [θ1=" << angles[0] << "°, θ2=" << angles[1] << "°, θ3=" << angles[2] << "°]\n";
    } catch (const std::domain_error& e) {
        std::cerr << "Target (" << x << ", " << y << ", " << z << ") is unreachable.\n";
    }

    return 0;
}
