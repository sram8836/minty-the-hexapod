#include "Brain.h"

// Constructor
Brain::Brain( Controller* controller, GaitType gaitType )
: linearMag ( 0.0f ),
  linearAngle ( 0.0f*M_PI ),
  rotationalVel ( 0.0f ),
  centralStepPercent ( 0.0f ),
  gaitParams(GaitParameters[gaitType]),
  controller( controller )
{   
    // Create state transmitter
    stateTransmitter = new StateTransmitter();

    // Create leg objects
    for (int i=0; i<legConfig.size(); i++) {
        legs.push_back(new Leg(i, legConfig[i], stateTransmitter));
    }

    std::cout << "Brain created" << std::endl;

    float f, l, r;

    while (true) {
        std::tie(f, l, r) = controller->getVelocities();
        updateVelocity(f, l, r);
        updateLegs();
        touchState = stateTransmitter->getTouchState();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    }
}


Brain::~Brain() {

    for (Leg* leg : legs) {
        delete leg;
    }

    std::cout << "Brain destroyed" << std::endl;
}


void Brain::updateLegs() {

    // Advance stepPercent according to velocity and previous stepPercent
    centralStepPercent += 1.0f;
    // centralStepPercent += (2.0f*linearMag) / (stepLength*updateFrequency);
    centralStepPercent = centralStepPercent > 100.0f ? centralStepPercent - 100.0f : centralStepPercent;
    
    // Update legs according to gait paramupdateFrequencyeters
    for (int i = 0; i<legs.size(); i += gaitParams.legsPerStep) {

        float movement_index = i / gaitParams.legsPerStep;
        float stepPercent = centralStepPercent + 100.0f*movement_index*(gaitParams.phaseDelay/360.0f);
        stepPercent = stepPercent > 100.0f ? stepPercent - 100.0f : stepPercent;

        for (int j = 0; j<gaitParams.legsPerStep; j++) {
            int legIndex = gaitParams.legSequence[i + j];
            legs[legIndex]->setStepPercent(stepPercent);
        }
    }

    // TODO: implement duty cycle logic

    stateTransmitter->sendAngles();
}


void Brain::updateVelocity( float forwardVel, float lateralVel, float rotationalVel ) {

    // Calculate sum angle and magnitude of linear velocities 
    float linearMag = std::sqrt(forwardVel * forwardVel + lateralVel * lateralVel);
    float linearAngle = std::atan2(-lateralVel, forwardVel);

    // Update leg trajectory if changed
    if (this->linearAngle != linearAngle) {
        this->linearAngle = linearAngle;
        
        for (Leg* leg : legs) {
            leg->setStepAngle(linearAngle);
        }
    }

    if (this->linearMag != linearMag) {
        this->linearMag = linearMag;
        
        for (Leg* leg : legs) {
            leg->setStepSize(linearMag);
        }
    }
}