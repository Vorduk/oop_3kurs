#include <iostream>
#include "SimulationEngine.h"
#include "SimulationModel.h"

int main()
{
    std::unique_ptr<SimulationModel> simulation_model = std::make_unique<SimulationModel>(25.0, 60.0, 70.0);
    std::unique_ptr<SimulationEngine> simulation_engine = std::make_unique<SimulationEngine>(*simulation_model);
    simulation_engine->start();
}


