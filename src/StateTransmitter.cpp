#include "StateTransmitter.h"


StateTransmitter::StateTransmitter( float txFrequency )
{   
    port = SerialPort("/dev/tty.usbmodem1101", B9600);
    // Angus: usbmodem14201
    // Sid  : usbmodem1101

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

std::vector<int> StateTransmitter::getTouchState()
{   
    std::vector<int> touchStates;
    std::string msg = port.readString();

    const std::string prefix = "touch:";
    const size_t prefixLength = prefix.length();

    // Validate starts with "touch:"
    if (msg.size() <= prefixLength || msg.compare(0, prefixLength, prefix) != 0) {
        return touchStates;
    }

    // Extract data component
    std::string data;
    for (size_t i = prefixLength; i < msg.size(); ++i) {
        data += msg[i];
    }

    std::stringstream ss(data);
    std::string token;

    while (std::getline(ss, token, ';')) {
        try {
            touchStates.push_back(std::stoi(token));
        } catch (...) {
            std::cout << "Error parsing touch state value: " << token << std::endl;
        }
    }

    return touchStates;
}
