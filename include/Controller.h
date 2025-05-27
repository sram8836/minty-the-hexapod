#pragma once

#include <thread>
#include <atomic>
#include <tuple>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

class Controller {
public:
    Controller();
    ~Controller();

    std::tuple<float, float, float> getVelocities() const;

private:
    std::atomic<float> forwardVel;
    std::atomic<float> lateralVel;
    std::atomic<float> rotationalVel;

    std::thread inputThread;
    std::atomic<bool> running;

    void inputLoop();
};
