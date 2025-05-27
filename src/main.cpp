#include "Brain.h"
#include "StateTransmitter.h"
#include "Controller.h"
#include "Leg.h"

int main() {

    Controller* controller = new Controller();
    Brain* hexapod = new Brain(controller);

    return 0;
}
