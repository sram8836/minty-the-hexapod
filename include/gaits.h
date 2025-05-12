#ifndef GAITS_H
#define GAITS_H

#include <vector>

const int numLegs = 6;

enum GaitType {
    TRIPOD,
    WAVE,
    RIPPLE,
    QUADRUPED
};

struct GaitParameterSet {
    double phaseDelay; // degrees
    double dutyCycle; // push percent (% of step on ground)
    double period; // seconds
    int legsPerStep;
    int legSequence[numLegs];
};

std::vector<std::pair<GaitType, GaitParameterSet>> GaitParameters = {
    {TRIPOD, {
        180.0f,
        50.0f,
        2.0f,
        3,
        {0,2,4,1,3,5},
    },},
    {WAVE, {
        120.0f,
        33.3f,
        2.0f,
        2,
        {0,3,1,4,2,5},
    },},
};

#endif