#include <iostream>
#include "Brain.h"

int main() {

    Brain hexapod;

    std::cout << "Hexapod" << std::endl;

    hexapod.inputGait();
    hexapod.update();


    return 0;
}