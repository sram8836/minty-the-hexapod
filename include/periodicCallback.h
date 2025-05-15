#pragma once

#include <thread>
#include <chrono>
#include <atomic>

using CallbackFunction = void(*)();

// Triggers supplied callback after specified period duration (milliseconds)
void registerPeriodicCallback(int aPeriod, CallbackFunction apCallback) {
    auto period = std::chrono::milliseconds(1000 * aPeriod);  // 100 Hz = every 10 ms

    while (1) {
        auto start = std::chrono::steady_clock::now();
        apCallback();
        std::this_thread::sleep_until(start + period);
    }
}
