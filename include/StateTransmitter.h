#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <array>
#include "PeriodicCallback.h"
#include "SerialPort.h"


class StateTransmitter {
    public:
        // Constructor
        StateTransmitter( float txFrequency = 50.0f );

        // Destructor
        ~StateTransmitter();

        // Methods
        void updateAngles( int legIndex, std::vector<float> values );

    private:
        // Private members
        std::vector<float> servoAngles;
        SerialPort port;
        static const int updateSize = 3;

        // Private Helper Methods
        void sendAngles();
};
