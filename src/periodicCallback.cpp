#include "PeriodicCallback.h"

// Triggers supplied callback after specified period duration (milliseconds)
void registerPeriodicCallback(float frequency, CallbackFunction callbackPtr) {

    int period_ms = static_cast<int>(1000.0f / frequency);
    auto period = std::chrono::milliseconds(period_ms);  // 100 Hz = every 10 ms

    while (1) {
        auto start = std::chrono::steady_clock::now();
        callbackPtr();
        std::this_thread::sleep_until(start + period);
    }
}
