#include <iostream>
#include "HexapodController.h"


// Constructor
HexapodController::HexapodController() {
    // Initialise legs vector
    std::cout << "HexapodController Created!" << std::endl;
    legs.resize(6, 0.0f); // Initialising 6 legs as 0.0
}

// Destructor
HexapodController::~HexapodController() {
    std::cout << "HexapodController Destroyed!" <<std::endl;
}

// Private Methods
void HexapodController::updateLegs() {
    std::cout << "Updating Legs..." << std::endl;

}

void HexapodController::setGait(int gaitType) {
    std::cout << "Setting Gait as Type " << gaitType << std::endl;
    this->gaitType = gaitType;
}

// Public Methods
void HexapodController::inputGait() {
    
    while (true) {
        
        std::cout << "Enter gait type: ";
        std::cin >> gaitType;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer." << std::endl;
        }
        else {
            break;
        }
    }

    setGait(gaitType);
}

void HexapodController::checkLegs() {
    std::cout << "Checking Legs" << std::endl;
}

void HexapodController::viewHexapod() {
    std::cout << "Visualising Hexapod" << std::endl;
}

