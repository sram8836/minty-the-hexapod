#include "PeriodicCallback.h"


PeriodicCallback::PeriodicCallback(float frequency, CallbackFunction callbackPtr) {
    std::thread([this, frequency, callbackPtr]() {
        int period_ms = static_cast<int>(1000.0f / frequency);
        auto period = std::chrono::milliseconds(period_ms);

        while (!stopFlag) {
            auto start = std::chrono::steady_clock::now();
            callbackPtr();
            std::this_thread::sleep_until(start + period);
        }
    }).detach();
}


PeriodicCallback::~PeriodicCallback() {
    stopFlag = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
