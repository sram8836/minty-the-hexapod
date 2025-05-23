#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <array>


enum LegState {
    INIT,
    SWING,
    SLIDE,
    FINISH,
};

class Leg {
    public:
        // Constructor
        Leg();

        // Destructor
        ~Leg();

        // Methods
        std::string getState();

        float getStepProgress();

        std::array<float, 3> getBasePosition();

        void setBasePosition(const std::array<float, 3>& pos);

        void updateState( LegState nextState, float nextCycleDuration);

        void advanceState();

        void updateFootTrajectory( std::vector<float> stepFunction);

        void step(char command);

    private:
        std::array<float, 3> basePosition {0.0f, 0.0f, 0.0f};
        std::array<float, 3> jointAngles;
        LegState currState;
        LegState nextState;
        float stepProgress;
        float cycleDuration;
        float nextCycleDuration;
        float sampleRate;
        
        void getJointAngles();

};
