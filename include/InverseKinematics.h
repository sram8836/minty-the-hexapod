#pragma once
#include <array>
#include <cmath>
#include <stdexcept>

class InverseKinematics {
public:
    static std::array<float, 3> solve(float x, float y, float z);
};
