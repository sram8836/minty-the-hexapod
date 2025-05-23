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
        Leg(float aBaseAngle);

        // Destructor
        ~Leg();

        // Methods
        std::string getState();

        float getStepProgress();

        std::array<float, 3> getBasePosition();

        std::array<float, 3> getJointAngles();

        void setBasePosition(const std::array<float, 3>& pos);

        void updateState( LegState nextState, float nextCycleDuration);

        void advanceState();

        void updateFootTrajectory( std::vector<float> stepFunction);

        std::array<float, 3> step(char command);

        void visualise();

        void test_step(float y);

    private:
        std::array<float, 3> basePosition;
        float baseAngle;
        std::array<float, 3> jointAngles;
        LegState currState;
        LegState nextState;
        float stepProgress;
        float cycleDuration;
        float nextCycleDuration;
        float sampleRate;

};
