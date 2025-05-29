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
        const float updateFrequency = 50.0f;
        const int sleepDuration = static_cast<int>(1000.0f / updateFrequency);
        const std::vector<float> legConfig = {
            M_PI/4, -M_PI/4, 0, 0, -M_PI/4, M_PI/4}; // All RHS servos reversed
        
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

        // Methods
        void updateLegs();

        void updateVelocity( float forwardVel, float lateralVel, float rotationalVel );

        float getFlipFactor( int i );
};
