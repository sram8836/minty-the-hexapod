#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "gaits.h"
#include "Leg.h"
#include "periodicCallback.h"

class Brain {
    public:
        // Constructor
        Brain( GaitType aGateType = TRIPOD );

        // Destructor
        ~Brain();

        // Methods
        void inputGait();

        void registerTouch( int leg ); // Callback for when sensor of specific leg touches ground

        void updateCommand( int cmd );

        // Fault Tolerance
        void checkLegs(); // Check if legs functioning properly

        // Visualisation
        void viewHexapod(); // For debugging

    private:
        // Private members
        std::vector<Leg> legs;
        GaitParameterSet gaitParams;

        // Private Helper Methods
        void updateLegs();
        void setGait( GaitType aNewGaitType );
        const int callbackPeriod = 10; // milliseconds
};
