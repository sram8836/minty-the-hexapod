#pragma once

#include <vector>
#include <string>
#include <iostream>


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

        float getStepProgress();

        void updateState( LegState nextState, float nextCycleDuration);

        void updateFootTrajectory( std::vector<float> stepFunction);

        void step(double dir_angle);

    private:
        LegState currState;
        LegState nextState;
        std::vector<float> jointAngles;
        std::vector<float> stepFunction;
        float cycleDuration;
        float nextCycleDuration;
        float sampleRate;
        
        void getJointAngles();

};
