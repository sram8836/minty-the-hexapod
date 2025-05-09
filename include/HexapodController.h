// HexapodController.h

#pragma once

#include <vector>
#include <string>

class HexapodController {
    public:
        // Constructor
        HexapodController();

        // Destructor
        ~HexapodController();

        // Methods
        void inputGait();

        // Fault Tolerance
        void checkLegs(); // Check if legs functioning properly

        // Visualisation
        void viewHexapod(); // For debugging

    private:
        // Private members
        std::vector<float> legs;
        int gaitType;

        // Private Helper Methods
        void updateLegs();
        void setGait(int gaitType);
};