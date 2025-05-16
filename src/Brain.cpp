#include "Brain.h"


void myCallback() {
    std::cout << "Callback triggered at " << std::chrono::steady_clock::now().time_since_epoch().count() << "\n";
}

// Constructor
Brain::Brain( GaitType aGateType ):
    gaitParams( GaitParameters[aGateType] )
{
    std::thread loopThread(registerPeriodicCallback, callbackPeriod, myCallback);
    legs.resize(6, Leg());
    std::cout << "Brain Created!" << std::endl;
}

// Destructor
Brain::~Brain() {
    std::cout << "Brain Destroyed!" <<std::endl;
}


void Brain::setGait( GaitType aNewGaitType ) {
    std::cout << "Setting Gait as Type " << aNewGaitType << std::endl;
    this->gaitParams = GaitParameters[aNewGaitType];
}


void Brain::viewHexapod() {
    std::cout << "Visualising Hexapod" << std::endl;
}

