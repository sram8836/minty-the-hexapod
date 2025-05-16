#include "ServoDriver.h"


// void myCallback() {
//     std::cout << "Callback triggered at " << std::chrono::steady_clock::now().time_since_epoch().count() << "\n";
// }

// Constructor
ServoDriver::ServoDriver()
{
    // std::thread loopThread(registerPeriodicCallback, callbackPeriod, myCallback);
    std::cout << "ServoDriver Created!" << std::endl;
    servoAngle.resize(18, 0.0f);
}

// Destructor
ServoDriver::~ServoDriver() {
    std::cout << "ServoDriver Destroyed!" <<std::endl;
}


void ServoDriver::updateAngles( int num, int* index, float* values ) {
    std::cout << "Updating Angles" << std::endl;
    for (int i = 0; i < num; i++) {
        int curr_index = index[i];
        servoAngle[curr_index] = values[i];
    };
}


void ServoDriver::sendAngles() {
    std::cout << "Sending Angles" << std::endl;
}

