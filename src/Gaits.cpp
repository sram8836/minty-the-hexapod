// Gaits.cpp
#include "Gaits.h"

std::unordered_map<GaitType, GaitParameterSet> GaitParameters = {
    {TRIPOD, {
        180.0f,
        50.0f,
        2.0f,
        3,
        {1,2,5,0,3,4}, // 0, 2, 4, 1, 3, 5
    },},
    {RIPPLE, {
        60.0f,
        83.3f,
        2.0f,
        1,
        // {0,3,1,5,2,4} - "working"
        {0,5,1,4,3,2}, // 1, 4, 0, 5, 3, 2
    },},
    {BIRIPPLE, {
        120.0f,
        66.6f,
        2.0f,
        2,
        {0,3,1,5,2,4},
    },},
    {DANCE, {
        180.0f,
        50.0f,
        2.0f,
        3,
        {0,2,4,1,3,5},
    },},
};