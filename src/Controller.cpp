#include "Controller.h"


Controller::Controller()
: forwardVel(0.0f), lateralVel(0.0f), rotationalVel(0.0f), running(true)
{   
    std::cout << "Controller created" << std::endl;
    currGait = GaitParameters[TRIPOD];
    height = -70.0f;
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
                case '1': {
                    // TRIPOD Gait
                    currGait = GaitParameters[TRIPOD];
                    std::cout << "Switching to TRIPOD" << std::endl;
                    break;
                }
                case '2': {
                    // RIPPLE Gait
                    currGait = GaitParameters[RIPPLE];
                    std::cout << "Switching to RIPPLE" << std::endl;
                    break;
                }
                case '3': {
                    // BIRIPPLE Gait
                    currGait = GaitParameters[BIRIPPLE];
                    std::cout << "Switching to BIRIPPLE" << std::endl;
                    break;
                }
                case '4': {
                    // WAVE Gait
                    currGait = GaitParameters[WAVE];
                    std::cout << "Switching to WAVE" << std::endl;
                    break;
                }
                case '5': {
                    // DANCE Gait
                    currGait = GaitParameters[DANCE];
                    std::cout << "Switching to DANCE" << std::endl;
                    break;
                }
                case '+': {
                    // Increase height
                    if (height >= -180 + HEIGHT_INCREMENT) {
                        height -= HEIGHT_INCREMENT;
                    }
                    std::cout << "Decreasing height to " << height << std::endl;
                    break;
                }
                case '-': {
                    // Decrease height
                    if (height <= -40 - HEIGHT_INCREMENT) {
                        height += HEIGHT_INCREMENT;
                    }
                    std::cout << "Increasing height to " << height << std::endl;
                    break;
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

float Controller::getHeight() {
    return height;
}

void Controller::printVelocities() {

    std::cout << "Forward velocity: " << forwardVel.load()
        << ", lateral velocity: " << lateralVel.load()
        << ", rotational velocity: " << rotationalVel.load() << std::endl;
}
