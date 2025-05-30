#include "Controller.h"


Controller::Controller()
: forwardVel(0.0f), lateralVel(0.0f), rotationalVel(0.0f), running(true)
{   
    std::cout << "Controller created" << std::endl;
    switchGait(0);
    inputThread = std::thread(&Controller::inputLoop, this);
}


Controller::~Controller() {
    running = false;
    if (inputThread.joinable())
    {
        inputThread.join();
    }
    
    std::cout << "Controller destroyed" << std::endl;
}


std::tuple<float, float, float> Controller::getVelocities() const {
    return {forwardVel.load(), lateralVel.load(), rotationalVel.load()};
}



// stdin to non-blocking
void Controller::inputLoop() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // turn off canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // non-blocking
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    while (running) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            switch (ch) {
                case 's': case 'S':
                    forwardVel.store(0.0f);
                    lateralVel.store(0.0f);
                    rotationalVel.store(0.0f);
                    printVelocities();
                    break;
                case 'w': case 'W': {
                    if (lateralVel.load() <= LINEAR_VEL_LIMIT - LINEAR_INCREMENT) {
                            forwardVel.store(forwardVel.load() + LINEAR_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case 'z': case 'Z': {
                    if (lateralVel.load() >= -LINEAR_VEL_LIMIT + LINEAR_INCREMENT) {
                            forwardVel.store(forwardVel.load() - LINEAR_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case 'd': case 'D': {
                    if (lateralVel.load() <= LINEAR_VEL_LIMIT - LINEAR_INCREMENT) {
                            lateralVel.store(lateralVel.load() + LINEAR_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case 'a': case 'A': {
                    if (lateralVel.load() >= -LINEAR_VEL_LIMIT + LINEAR_INCREMENT) {
                            lateralVel.store(lateralVel.load() - LINEAR_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case 'q': case 'Q': {
                    if (rotationalVel.load() <= ROTATIONAL_VEL_LIMIT - ROTATIONAL_INCREMENT) {
                            rotationalVel.store(rotationalVel.load() + ROTATIONAL_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case 'e': case 'E': {
                    if (rotationalVel.load() >= -ROTATIONAL_VEL_LIMIT + ROTATIONAL_INCREMENT) {
                            rotationalVel.store(rotationalVel.load() - ROTATIONAL_INCREMENT);
                        }
                    }
                    printVelocities();
                    break;
                case '0': {
                    // TRIPOD Gait
                    switchGait(0);
                }
                case '1': {
                    // RIPPLE Gait
                    switchGait(1);
                }
                case '2': {
                    // BIRIPPLE Gait
                    switchGait(2);
                }
                case '3': {
                    // DANCE Gait
                    switchGait(3);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);    // restore terminal
}


GaitParameterSet Controller::getGait() {
    return currGait;
}

void Controller::switchGait(int newGait) {
    
    if (newGait == 0) {
        currGait = GaitParameters[TRIPOD];
    }
    else if (newGait == 1) {
        currGait = GaitParameters[RIPPLE];
    }
    else if (newGait == 2) {
        currGait = GaitParameters[BIRIPPLE];
    }
    else if (newGait == 3) {
        currGait = GaitParameters[DANCE];
    }
}


void Controller::printVelocities() {

    std::cout << "Forward velocity: " << forwardVel.load()
        << ", lateral velocity: " << lateralVel.load()
        << ", rotational velocity: " << rotationalVel.load() << std::endl;
}
