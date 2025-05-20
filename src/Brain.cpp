#include "Brain.h"

Brain* brainPtr = nullptr;

void myCallback() {
    if (brainPtr) {
        brainPtr->update();
    }
}

// Constructor
Brain::Brain(GaitType aGateType)
    : gaitParams(GaitParameters[aGateType])
{
    legs.resize(numLegs, Leg());
    legPhaseOffsets.resize(numLegs);
    legProgress.resize(numLegs, 0.0);

    for (int i = 0; i < numLegs; ++i) {
        legPhaseOffsets[i] = (gaitParams.legSequence[i] * gaitParams.phaseDelay) / 360.0;
    }

    std::thread loopThread(registerPeriodicCallback, callbackPeriod, myCallback);
    loopThread.detach(); //avoid blocking the constructor

    brainPtr = this;

    std::cout << "Brain Created!" << std::endl;
}

// Destructor
Brain::~Brain() {
    std::cout << "Brain Destroyed!" << std::endl;
}

void Brain::updateState(BrainState newState) {
    state = newState;
}

void Brain::setGait(GaitType aNewGaitType) {
    std::cout << "Setting Gait as Type " << aNewGaitType << std::endl;
    this->gaitParams = GaitParameters[aNewGaitType];

    for (int i = 0; i < numLegs; ++i) {
        legPhaseOffsets[i] = (gaitParams.legSequence[i] * gaitParams.phaseDelay) / 360.0;
    }
}

void Brain::viewHexapod() {
    std::cout << "Visualising Hexapod" << std::endl;
}

void Brain::inputGait() {
    std::cout << "Input Gait: TRIPOD(0), WAVE(1): ";
    int g;
    std::cin >> g;

    if (g == 0) setGait(TRIPOD);
    else if (g == 1) setGait(WAVE);
    else std::cout << "Invalid gait\n";
}

void Brain::registerTouch(int leg) {
    std::cout << "Touch registered on leg " << leg << std::endl;
}

void Brain::checkLegs() {
    std::cout << "Checking legs for fault..." << std::endl;
}

void Brain::update() {
    updateLegs();
    resyncLegs();
}

void Brain::updateLegs() {
    for (int i = 0; i < numLegs; ++i) {
        double progress = legs[i].getStepProgress(); // returns 0.0 to 1.0
        legProgress[i] = progress;

        if (progress >= legPhaseOffsets[i]) {
            legs[i].step(0.0); 
        }
    }
}

void Brain::resyncLegs() {
    double avg = 0.0;
    for (double p : legProgress) avg += p;
    avg /= numLegs;

    for (int i = 0; i < numLegs; ++i) {
        double error = legProgress[i] - avg;

        if (std::abs(error) > 0.05) {
            legPhaseOffsets[i] -= 0.5 * error;

            if (legPhaseOffsets[i] < 0) legPhaseOffsets[i] += 1.0;
            if (legPhaseOffsets[i] > 1) legPhaseOffsets[i] -= 1.0;
        }
    }
}
