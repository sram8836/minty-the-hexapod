#include "Brain.h"


// Constructor
Brain::Brain():
    gaitType( TRIPOD )
{
    // Initialise legs vector
    std::cout << "Brain Created!" << std::endl;
    legs.resize(6, 0.0f); // Initialising 6 legs as 0.0
}

// Destructor
Brain::~Brain() {
    std::cout << "Brain Destroyed!" <<std::endl;
}

// Private Methods
void Brain::updateLegs() {
    std::cout << "Updating Legs..." << std::endl;

}

void Brain::setGait(int gaitType) {
    std::cout << "Setting Gait as Type " << gaitType << std::endl;
    this->gaitType = gaitType;
}

// Public Methods
void Brain::inputGait() {
    
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

void Brain::checkLegs() {
    std::cout << "Checking Legs" << std::endl;
}

void Brain::viewHexapod() {
    std::cout << "Visualising Hexapod" << std::endl;
}

