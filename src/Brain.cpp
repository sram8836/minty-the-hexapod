#include "Brain.h"

// Constructor
Brain::Brain( Controller* controller, GaitType gaitType )
: linearMag ( 0.0f ),
  linearAngle ( 0.0f ),
  rotationalVel ( 0.0f ),
  centralStepPercent ( 0.0f ),
  gaitParams( GaitParameters[gaitType] ),
  controller( controller )
{   
    // Create state transmitter
    stateTransmitter = new StateTransmitter();

    // Create leg objects
    for (int i=0; i<legConfig.size(); i++) {
        legs.push_back(new Leg(i, legConfig[i], stateTransmitter));
    }

    touchState = {0,0,0,0,0,0};

    std::cout << "Brain created" << std::endl;

    while (true) {
        updateTouchState();
        updateVelocity();
        updateLegs();

        if (centralStepPercent == 0.75 && !(touchState[4] && touchState[5]) ) {
            std::cout << "Cliff detected. Cancelling remote control" << std::endl;
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


void Brain::updateTouchState() {
    std::vector<int> newTouch = stateTransmitter->getTouchState();

    // Shift new value into buffer
    touchBuffer.insert(touchBuffer.begin(), newTouch);
    
    // Shift old value out of buffer if full
    if (touchBuffer.size() >= bufferSize) {
        touchBuffer.pop_back();
    }

    // And touch sensor value for each leg
    const int numLegs = 6;
    int isOne[numLegs] = {0,0,0,0,0,0};
    int isZero[numLegs] = {0,0,0,0,0,0};

    for (int i=0; i<numLegs; i++) {
        isOne[i] = touchBuffer[0][i];
        isZero[i] = !touchBuffer[0][i];

        for (int j=0; j<touchBuffer.size(); j++) {
            isOne[i] &= touchBuffer[j][i];
            isZero[i] &= !touchBuffer[j][i];
        }
    }

    for (int i=0; i<numLegs; i++) {
        if (isOne[i]) {
            if (touchState[i] == 0) {
                legs[i]->registerTouch();
            }
            touchState[i] = 1;
        }
        else if (isZero[i]) {
            if (touchState[i] == 1) {
                legs[i]->registerUntouch();
            }
            touchState[i] = 0;
        }
    }
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
    }
}