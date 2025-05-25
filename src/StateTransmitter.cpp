#include "StateTransmitter.h"


StateTransmitter::StateTransmitter( float txFrequency )
{   
    port = SerialPort("/dev/tty.usbmodem14101", B9600);

    if (!port.init()) {
        std::cout << "Could not connect to serial." << std::endl;
        return;
    }

    servoAngles.resize(18, 0.0f);
    registerPeriodicCallback(txFrequency, [this]() { this->sendAngles(); });

    std::cout << "StateTransmitter Created!" << std::endl;
}


StateTransmitter::~StateTransmitter()
{
    std::cout << "StateTransmitter Destroyed!" <<std::endl;
}


// Updates n values (updateSize) in the legIndex-th set of n values
void StateTransmitter::updateAngles( int legIndex, std::vector<float> values )
{
    for (int i = 0; i < updateSize; i++) {
        servoAngles[legIndex*updateSize + i] = values[i];
    }
}


void StateTransmitter::sendAngles()
{
    std::cout << "Sending Angles" << std::endl;

    std::string cmd = "angles:";

    for (int i = 0; i < servoAngles.size(); i++) {
        cmd += std::to_string(servoAngles[i]) + ";";
    }
    
    cmd[-1] = '\n';
    port.send(cmd);
}
