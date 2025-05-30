#pragma once

#include <unordered_map>
#include <map>

const int numLegs = 6;

enum GaitType {
    TRIPOD,
    RIPPLE,
    BIRIPPLE,
    DANCE
};

struct GaitParameterSet {
    double phaseDelay; // degrees
    double dutyCycle; // push percent (% of step on ground)
    double period; // seconds
    int legsPerStep;
    int legSequence[numLegs];
};

extern std::unordered_map<GaitType, GaitParameterSet> GaitParameters;
