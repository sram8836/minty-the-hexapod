// Leg.h

#pragma once

#include <vector>
#include <string>

class Leg {
    public:
        // Constructor
        Leg();

        // Destructor
        ~Leg();

        // Methods
        void getStatus();

        void setMaxServoSpeed(double maxRotationalVelocity);

        void goToPose();

        void step(double dir_angle);

    private:
        void getJointAngles();
};