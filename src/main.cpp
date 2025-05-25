#include "Leg.h"
#include "StateTransmitter.h"

int main() {

    StateTransmitter txr = StateTransmitter();
    Leg leg = Leg(0, -M_PI/4, &txr);
    leg.saveTrajectory();

    return 0;
}