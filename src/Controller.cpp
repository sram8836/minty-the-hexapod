#include "Controller.h"

constexpr float VELOCITY_INCREMENT = 0.01f;

Controller::Controller()
: forwardVel(0.0f), lateralVel(0.0f), rotationalVel(0.0f), running(true)
{
    inputThread = std::thread(&Controller::inputLoop, this);
}

Controller::~Controller() {
    running = false;
    if (inputThread.joinable())
        inputThread.join();
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
                case 'w': case 'W':
                    forwardVel.store(forwardVel.load() + VELOCITY_INCREMENT);
                    break;
                case 's': case 'S':
                    forwardVel.store(0.0f);
                    lateralVel.store(0.0f);
                    rotationalVel.store(0.0f);
                    break;
                case 'z': case 'Z':
                    forwardVel.store(forwardVel.load() - VELOCITY_INCREMENT);
                    break;
                case 'a': case 'A':
                    lateralVel.store(lateralVel.load() + VELOCITY_INCREMENT);
                    break;
                case 'd': case 'D':
                    lateralVel.store(lateralVel.load() - VELOCITY_INCREMENT);
                    break;
                case 'q': case 'Q':
                    rotationalVel.store(rotationalVel.load() + VELOCITY_INCREMENT);
                    break;
                case 'e': case 'E':
                    rotationalVel.store(rotationalVel.load() - VELOCITY_INCREMENT);
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);    // restore terminal
}
