#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"
#include "ClimateManager.h"
#include "ConfigManager.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System - Step 4" << std::endl;
    std::cout << "With Climate Control" << std::endl;
    std::cout << "========================================" << std::endl;

    // Модель данных
    SimulationModel simulationModel(22.0, 65.0, 45.0);

    // Датчики
    TemperatureSensor tempSensor(&simulationModel);
    AirHumiditySensor humiditySensor(&simulationModel);
    SoilMoistureSensor soilSensor(&simulationModel);

    // IOManager и датчики
    IOManager ioManager;
    ioManager.addSensor(&tempSensor);
    ioManager.addSensor(&humiditySensor);
    ioManager.addSensor(&soilSensor);

    // Менеджеры
    auto climateManager = std::make_shared<ClimateManager>();
    auto configManager = std::make_shared<ConfigManager>();

    // Можно изменить целевые параметры
    configManager->setTargetParameter("temperature", 23.0);
    configManager->setTargetParameter("air_humidity", 60.0);
    configManager->setTargetParameter("soil_moisture", 50.0);

    // 5. Создаем движок и настраиваем
    SimulationEngine engine(simulationModel, ioManager);
    engine.setClimateManager(climateManager);
    engine.setConfigManager(configManager);

    // 6. Запускаем
    engine.start();

    return 0;
}