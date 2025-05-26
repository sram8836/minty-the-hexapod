#include "Brain.h"
#include "StateTransmitter.h"

int main() {
    Brain hexapod = Brain();
    hexapod.updateVelocity(50.0f, 50.0f, 0.0f);
    return 0;
}
