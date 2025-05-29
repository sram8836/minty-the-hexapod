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
        std::vector<std::vector<int>> touchBuffer;
        GaitParameterSet gaitParams;
        StateTransmitter* stateTransmitter;
        Controller *controller;
        float linearMag;
        float linearAngle;
        float rotationalVel;
        float centralStepPercent;

        // Methods
        void updateLegs();

        void updateTouchState();

        void updateVelocity();

        float getFlipFactor( int i );
};
