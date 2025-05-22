#include "InverseKinematics.h"

std::array<float, 3> InverseKinematics::solve(float x, float y, float z) {
    // DH constants
    const float l1 = 0.25f;  // link 1
    const float l2 = 0.5f;   // link 2
    const float l3 = 0.5f;   // link 3

    // joint 1, axis rotates about z
    float theta1 = std::atan2(y, x);

    // convert input coordinates to range
    float x1 = std::sqrt((x*x)+(y*y)) - l1;
    float z1 = z;

    // distance from joint 2
    float r = std::sqrt((x1*x1)+(z1*z1)); 

    // check reachability
    if ((r > (l2+l3)) || (r < std::abs(l2-l3))) {
        throw std::domain_error("Bad coordinates");
    }

    // Law of Cosines for elbow angle (theta3)
    float cosTheta3 = (r * r - l2 * l2 - l3 * l3) / (2 * l2 * l3);
    float theta3 = std::acos(cosTheta3);

    // Elbow-down configuration
    theta3 = -theta3;

    // Law of Cosines for shoulder angle (theta2)
    float k1 = l2 + l3 * std::cos(theta3);
    float k2 = l3 * std::sin(theta3);
    float theta2 = std::atan2(z1, x1) - std::atan2(k2, k1);

    return {theta1, theta2, theta3};
}
