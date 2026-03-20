#include <iostream>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"

int main() {


    // 1. Создаем модель данных
    SimulationModel simulationModel(22.0, 65.0, 45.0);

    // 2. Создаем датчики
    TemperatureSensor tempSensor(&simulationModel);
    AirHumiditySensor humiditySensor(&simulationModel);
    SoilMoistureSensor soilSensor(&simulationModel);

    // 3. Создаем IOManager и добавляем датчики
    IOManager ioManager;
    ioManager.addSensor(&tempSensor);
    ioManager.addSensor(&humiditySensor);
    ioManager.addSensor(&soilSensor);

    // 4. Создаем движок симуляции (передаем модель и IOManager)
    SimulationEngine engine(simulationModel, ioManager);

    engine.start();

    return 0;
}