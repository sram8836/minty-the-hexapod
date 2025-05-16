#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "periodicCallback.h"

class ServoDriver {
    public:
        // Constructor
        ServoDriver();

        // Destructor
        ~ServoDriver();

        // Methods
        void updateAngles(int num, int* index, float* values);

    private:
        // Private members
        std::vector<float> servoAngle;

        // Private Helper Methods
        void sendAngles();
};
