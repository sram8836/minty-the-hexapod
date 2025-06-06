#include "Brain.h"

// Constructor
Brain::Brain( Controller* controller, GaitType gaitType )
: linearMag ( 0.0f ),
  linearAngle ( 0.0f ),
  rotationalVel ( 0.0f ),
  centralStepPercent ( 0.0f ),
  gaitParams( GaitParameters[gaitType] ),
  controller( controller ),
  cliffCount( 0 ),
  touchState( {0,0,0,0,0,0} )
{   
    // Create state transmitter
    stateTransmitter = new StateTransmitter();

    // Create leg objects
    for (int i=0; i<legConfig.size(); i++) {
        legs.push_back(new Leg(i, legConfig[i], stateTransmitter));
    }

    std::cout << "Brain created" << std::endl;

    while (true) {
        updateVelocity();
        stabilise();
        updateLegs();

        if (checkForCliff()) {
            break;
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    }
}


Brain::~Brain() {

    for (Leg* leg : legs) {
        delete leg;
    }

    std::cout << "Brain destroyed" << std::endl;
}


int Brain::checkForCliff() {
    touchState = stateTransmitter->getTouchState();

    if (centralStepPercent >= 0.75 && !(touchState[4] && touchState[5]) ) {
        cliffCount += 1;
        if (cliffCount > 20) {
            std::cout << "Cliff detected. Cancelling remote control" << std::endl;
            return 1;
        }
    }
    else {
        cliffCount = 0;
    }
    return 0;
}


void Brain::updateLegs() {

    // Advance stepPercent according to velocity and previous stepPercent
    if (linearMag > maxStepSize) {
        centralStepPercent += linearMag / updateFrequency;
    }
    else {
        centralStepPercent += 2.0f;
    }

    // Wrap percentage
    if (centralStepPercent > 100.0f) {
        centralStepPercent -= 100.0f;
    }

    gaitParams = controller->getGait();

    for (int seqIndex = 0; seqIndex < numLegs; ++seqIndex) {
        int legIndex = gaitParams.legSequence[seqIndex];

        float offsetDegrees = gaitParams.phaseDelay * seqIndex;
        float legStepPercent = fmod(centralStepPercent + (offsetDegrees / 360.0f) * 100.0f, 100.0f);

        legs[legIndex]->setStepPercent(legStepPercent);
    }

    stateTransmitter->sendAngles();
}


float Brain::getFlipFactor( int i )
{   
    int isLeft = i == 0 || i == 3 || i == 4;
    int isRight = i == 2 || i == 1 || i == 5;
    return static_cast<float>(isLeft- isRight);
}


void Brain::updateVelocity() {

    float forwardVel, lateralVel, rotationalVel;
    std::tie(forwardVel, lateralVel, rotationalVel) = controller->getVelocities();

    for (int i = 0; i < legs.size(); i++) {
        float rotationAngle = -(legConfig[i] + M_PI);
        float rotationMag = getFlipFactor(i)*500*rotationalVel;

        // Add velocity component which is orthogonal to mount angle
        float totalForward = forwardVel + rotationMag*std::cos(rotationAngle);
        float totalLateral = lateralVel + rotationMag*std::sin(rotationAngle);

        // Calculate sum angle and magnitude of linear velocities 
        float linearMag = std::sqrt(totalForward * totalForward + totalLateral * totalLateral);
        float linearAngle = std::atan2(-totalLateral, totalForward);

        this->linearMag = linearMag;

        if (linearMag > maxStepSize) {
            linearMag = maxStepSize;
        }

        legs[i]->setStepAngle(linearAngle);
        legs[i]->setStepSize(linearMag);
        legs[i]->setZNom(controller->getHeight());
    }
}


void Brain::stabilise() {

    float roll = degToRad(orientation[0]);
    float pitch = degToRad(orientation[1]);
    float yaw = degToRad(orientation[2]);

    const float offsetXEnds = 63.0f;
    const float offsetXMid = 81.5f;
    const float offsetY = 83.5f;

    float legOffsets[6][2] = {
        {-offsetXEnds, -offsetY}, // Leg 0
        { offsetXEnds, -offsetY}, // Leg 1
        {  offsetXMid,     0.0f}, // Leg 2
        { -offsetXMid,     0.0f}, // Leg 3
        {-offsetXEnds,  offsetY}, // Leg 4
        { offsetXEnds,  offsetY}  // Leg 5
    };

    for (int i=0; i<legs.size(); ++i) {
        float x = legOffsets[i][0];
        float y = legOffsets[i][1];

        // Adjust zNom due to tilt

        float baseZ = legs[i]->getBaseHeight();
        float dz = - x * sinf(pitch) - y * sinf(roll);
        float newZNom = baseZ - dz;

        legs[i]->setZNom(newZNom);
    }
}