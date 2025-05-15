#pragma once

#include <vector>
#include <string>


enum LegState {
    INIT,
    PUSH,
    LIFT,
    STOP,
};


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
        std::string Status;
        std::vector<float> jointAngles;
        
    
        void getJointAngles();

};
