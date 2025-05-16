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
        void getState();

        void updateState( LegState nextState, float nextCycleDuration);

        void updateFootTrajectory( std::vector<float> stepFunction);

        void getStepProgress();

        void step(double dir_angle);

    private:
        std::string Status;
        std::vector<float> jointAngles;
        std::vector<float> stepFunction;
        float cycleDuration;
        float nextCycleDuration;
        LegState nextState;
        float sampleRate;
        
        void getJointAngles();

};
