#include <vector>
#include <iostream>
#include <string>
#include <gaits.h>


class Brain {
    public:
        // Constructor
        Brain();

        // Destructor
        ~Brain();

        // Methods
        void inputGait();

        void registerTouch(int leg); // Callback for when sensor of specific leg touches ground

        void updateCommand(int cmd);

        // Fault Tolerance
        void checkLegs(); // Check if legs functioning properly

        // Visualisation
        void viewHexapod(); // For debugging

    private:
        // Private members
        std::vector<float> legs;
        GaitType gaitType;

        // Private Helper Methods
        void updateLegs();
        void setGait(int gaitType);
};