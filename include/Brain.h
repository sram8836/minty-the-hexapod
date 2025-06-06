#pragma once
#include <array>
#include <iostream>
#include <cmath>
#include <fstream>
#include "Gaits.h"
#include "Leg.h"
#include "StateTransmitter.h"
#include "PeriodicCallback.h"
#include "Controller.h"

class Brain {
    public:
        // Constructor
        Brain( Controller* controller, GaitType gaitType = TRIPOD );

        // Destructor
        ~Brain();

    private:
        // Constants
        const int bufferSize = 5; // For touch buffer
        const float updateFrequency = 50.0f; // Hz
        const float maxStepSize = 150.0f; // mm
        const int sleepDuration = static_cast<int>(1000.0f / updateFrequency); // ms
        const std::vector<float> legConfig = {
            M_PI/4, -M_PI/4, 0, 0, -M_PI/4, M_PI/4}; // rad
        
        // Variables
        std::vector<Leg*> legs;
        std::vector<int> touchState;
        GaitParameterSet gaitParams;
        StateTransmitter* stateTransmitter;
        Controller *controller;
        float linearMag;
        float linearAngle;
        float rotationalVel;
        float centralStepPercent;
        int cliffCount;
        std::array<float, 3> orientation;

        // Methods
        void updateLegs();

        int checkForCliff();

        void stabilise();

        void updateVelocity();

        float getFlipFactor( int i );

        float radToDeg(float radians) { return radians * (180.0 / M_PI); };

        float degToRad(float degrees) { return degrees * (M_PI / 180.0); };
};
