#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "gaits.h"
#include "Leg.h"
#include "periodicCallback.h"

#include <fstream>

enum BrainState {
    START,
    MOVE,
    STOP,
};

class Brain {
    public:
        // Constructor
        Brain(GaitType aGaitType = TRIPOD);

        // Destructor
        ~Brain();

        // Methods
        void inputGait();
        void registerTouch(int leg); // Callback for when sensor of specific leg touches ground
        void updateState(BrainState newState);
        void checkLegs(); // Check if legs functioning properly
        void viewHexapod(); // For debugging
        void update(); // Called periodically by callback

    private:
        // Members
        BrainState state;
        std::vector<float> legAngles;
        std::vector<Leg> legs;
        GaitParameterSet gaitParams;
        std::vector<double> legPhaseOffsets;
        std::vector<double> legProgress;

        const int callbackPeriod = 10; // milliseconds

        // Helpers
        void updateLegs();
        void setGait(GaitType aNewGaitType);
        void resyncLegs();
        
        std::ofstream syncLog;
        int tickCount = 0;
        void logSyncData(); // for CSV leg progress

};
