#include <iostream>
#include "Simulation.h"

int main()
{
    std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(20.0, 70.0, 60.0);
    simulation->start();
}


