#pragma once
#include <array>
#include <iostream>
#include <cmath>
#include <fstream>
#include "Gaits.h"
#include "Leg.h"
#include "StateTransmitter.h"
#include "PeriodicCallback.h"


class Brain {
    public:
        // Constructor
        Brain( GaitType aGaitType = TRIPOD );

        // Destructor
        ~Brain();

        // Methods
        void updateVelocity( float forwardVel, float lateralVel, float rotationalVel );

    private:
        // Constants
        const float updateFrequency = 10.0f;
        const float stepLength = 100.0f;
        const std::vector<float> legConfig = {
            M_PI/4, -M_PI/4, 0, 0, -M_PI/4, M_PI/4}; // All RHS servos reversed
        
        // Variables
        std::vector<Leg*> legs;
        GaitParameterSet gaitParams;
        StateTransmitter* stateManager;
        PeriodicCallback* timer;
        float linearMag;
        float linearAngle;
        float rotationalVel;
        float centralStepPercent;

        // Methods
        void updateLegs();

        float getTurnFactor( int legIndex );
};
