#include "StateTransmitter.h"


StateTransmitter::StateTransmitter( float txFrequency )
{   
    port = SerialPort("/dev/tty.usbmodem14301", B9600);

    if (!port.init()) {
        std::cout << "Could not connect to serial." << std::endl;
        return;
    }

    servoAngles.resize(18, 0.0f);

    std::cout << "StateTransmitter Created!" << std::endl;
}


StateTransmitter::~StateTransmitter()
{
    std::cout << "StateTransmitter Destroyed!" <<std::endl;
}


// Updates n values (updateSize) in the legIndex-th set of n values
void StateTransmitter::updateAngles( int legIndex, std::vector<float> values )
{   
    std::lock_guard<std::mutex> lock(servoMutex);

    for (int i = 0; i < updateSize; i++) {
        this->servoAngles[legIndex*updateSize + i] = values[i];
    }
}


void StateTransmitter::sendAngles()
{
    std::string cmd = "angles:";

    std::lock_guard<std::mutex> lock(servoMutex);
    
    for (float angle : servoAngles) {
        cmd += std::to_string(angle) + ";";
    }
    
    if (!cmd.empty()) cmd.back() = '\n';
    port.send(cmd);
}
