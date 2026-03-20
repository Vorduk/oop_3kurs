#include <iostream>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System" << std::endl;
    std::cout << "========================================" << std::endl;

    SimulationModel simulationModel(22.0, 65.0, 45.0);

    TemperatureSensor tempSensor(&simulationModel);
    AirHumiditySensor humiditySensor(&simulationModel);
    SoilMoistureSensor soilSensor(&simulationModel);

    IOManager ioManager;
    ioManager.addSensor(&tempSensor);
    ioManager.addSensor(&humiditySensor);
    ioManager.addSensor(&soilSensor);

    SimulationEngine engine(simulationModel, ioManager);

    engine.start();

    return 0;
}