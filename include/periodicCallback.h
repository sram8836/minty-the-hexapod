#pragma once
#include <thread>
#include <chrono>
#include <atomic>

using CallbackFunction = std::function<void()>;

// Triggers supplied callback after specified period duration (milliseconds)
void registerPeriodicCallback(float frequency, CallbackFunction callbackPtr);
