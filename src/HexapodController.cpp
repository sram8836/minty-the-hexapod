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

void HexapodController::updateLegs() {
    std::cout << "Updating Legs..." << std::endl;
}

void HexapodController::setGait() {
    std::cout << "Setting Gait..." << std::endl;
}

void HexapodController::inputGait() {
    std::cout << "Input Gait:" << std::endl;
}

void HexapodController::checkLegs() {
    std::cout << "Checking Legs" << std::endl;
}

void HexapodController::viewHexapod() {
    std::cout << "Visualising Hexapod" << std::endl;
}

