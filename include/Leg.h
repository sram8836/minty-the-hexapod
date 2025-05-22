#pragma once

#include <vector>
#include <string>
#include <iostream>


enum LegState {
    INIT,
    PUSH,
    LIFT,
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
        LegState currState;
        LegState nextState;
        std::vector<float> jointAngles;
        std::vector<float> stepFunction;
        float cycleDuration;
        float nextCycleDuration;
        float sampleRate;
        
        void getJointAngles();

};
