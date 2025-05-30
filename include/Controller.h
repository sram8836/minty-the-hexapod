#pragma once

#include <thread>
#include <atomic>
#include <tuple>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Gaits.h"

class Controller {
public:
    Controller();
    ~Controller();

    // Methods
    GaitParameterSet getGait();

    float getHeight();

    std::tuple<float, float, float> getVelocities() const;

private:
    // Constants
    const float LINEAR_INCREMENT = 20.0f; // mm
    const float ROTATIONAL_INCREMENT = 0.1f; // mm
    const float LINEAR_VEL_LIMIT = 200.0f; // mm
    const float ROTATIONAL_VEL_LIMIT = 0.5f; // rad
    const float updateFrequency = 10.0f; // Hz
    const int sleepDuration = static_cast<int>(1000.0f / updateFrequency);
    const float HEIGHT_INCREMENT = 10.0f;

    // Variables
    std::atomic<float> forwardVel;
    std::atomic<float> lateralVel;
    std::atomic<float> rotationalVel;
    std::thread inputThread;
    std::atomic<bool> running;
    GaitParameterSet currGait;
    float height;
    
    // Methods
    void inputLoop();

    void printVelocities();
};
