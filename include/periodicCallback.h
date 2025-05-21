#pragma once

#include <thread>
#include <chrono>
#include <atomic>

using CallbackFunction = void(*)();

// Triggers supplied callback after specified period duration (milliseconds)
void registerPeriodicCallback(int aPeriod, CallbackFunction apCallback);