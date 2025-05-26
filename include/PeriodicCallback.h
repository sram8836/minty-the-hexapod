#pragma once
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

using CallbackFunction = std::function<void()>;

// Triggers supplied callback after specified period duration (milliseconds)
class PeriodicCallback {
    public:
        PeriodicCallback(float frequency, CallbackFunction callbackPtr);
        ~PeriodicCallback();

    private:
        std::atomic<bool> stopFlag{false};
};
