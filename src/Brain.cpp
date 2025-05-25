#include "Brain.h"


// Constructor
Brain::Brain( GaitType aGaitType )
: linearMag ( 0.0f ),
  linearAngle ( 0.0f ),
  rotationalVel ( 0.0f ),
  centralStepPercent ( 0.0f ),
  gaitParams(GaitParameters[aGaitType])
{   
    // Create state transmitter
    stateManager = new StateTransmitter();

    // Create leg objects
    for (int i=0; i<legConfig.size(); i++) {
        legs.push_back(new Leg(i, legConfig[i], stateManager));
    }

    registerPeriodicCallback(updateFrequency, [this]() { this->updateLegs(); });

    std::cout << "Brain created" << std::endl;
}


Brain::~Brain() {

    for (Leg* leg : legs) {
        delete leg;
    }

    std::cout << "Brain destroyed" << std::endl;
}


void Brain::updateLegs() {

    // Advance stepPercent according to velocity and previous stepPercent
    centralStepPercent += 1.0f; // time_step_size = total_duration / number_of_updates
    centralStepPercent = centralStepPercent > 100.0f ? centralStepPercent - 100.0f : centralStepPercent;
    
    // Update legs according to gait parameters
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

    stateManager->sendAngles();
}


void Brain::updateVelocity( float forwardVel, float lateralVel, float rotationalVel ) {

    // Calculate sum angle and magnitude of linear velocities 
    float linearMag = std::sqrt(forwardVel * forwardVel + lateralVel * lateralVel);
    float linearAngle = std::atan2(-lateralVel, forwardVel);

    // Update leg trajectory if changed
    if (this->linearAngle != linearAngle) {
        this->linearAngle = linearAngle;
        
        for (int i = 0; i<legs.size(); i++) {
            legs[i]->setStepAngle(linearAngle);
        }
    }

    if (this->linearMag != linearMag) {
        // TODO
    }

    if (this->rotationalVel != rotationalVel) {
        // TODO
    }
}
