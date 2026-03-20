#include <iostream>
#include <memory>
#include "SimulationEngine.h"
#include "SimulationModel.h"
#include "IOManager.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"
#include "ClimateManager.h"
#include "ScheduleConfig.h"
#include "SafetyProxyManager.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System - Lab 4" << std::endl;
    std::cout << "Patterns: Delegation, Proxy, Configuration" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // 1. Создаем модель как shared_ptr (она будет жить долго)
    auto simulationModel = std::make_shared<SimulationModel>(22.0, 65.0, 45.0);

    // 2. Создаем датчики с weak_ptr на модель
    auto tempSensor = std::make_shared<TemperatureSensor>(simulationModel);
    auto humiditySensor = std::make_shared<AirHumiditySensor>(simulationModel);
    auto soilSensor = std::make_shared<SoilMoistureSensor>(simulationModel);

    // 3. Создаем устройства
    auto heater = std::make_shared<Heater>();
    auto conditioner = std::make_shared<Conditioner>();
    auto humidifier = std::make_shared<AirHumidifier>();
    auto irrigation = std::make_shared<Irrigation>();
    auto ventilation = std::make_shared<Ventilation>();
    auto lamp = std::make_shared<Lamp>();

    // 4. Настройка IOManager (владеет shared_ptr)
    auto io_manager = std::make_shared<IOManager>();
    io_manager->addSensor(tempSensor);
    io_manager->addSensor(humiditySensor);
    io_manager->addSensor(soilSensor);
    io_manager->addDevice(heater);
    io_manager->addDevice(conditioner);
    io_manager->addDevice(humidifier);
    io_manager->addDevice(irrigation);
    io_manager->addDevice(ventilation);
    io_manager->addDevice(lamp);

    // 5. Реальный ClimateManager с делегированием
    auto realClimateManager = std::make_shared<ClimateManager>();

    // Паттерн Delegation: разные регуляторы для разных параметров
    auto pidRegulator = std::make_shared<PidRegulator>(2.0, 0.1, 0.5);
    auto onOffRegulator = std::make_shared<OnOffRegulator>(2.0);

    realClimateManager->setRegulator("temperature", pidRegulator);
    realClimateManager->setRegulator("air_humidity", onOffRegulator);
    realClimateManager->setRegulator("soil_moisture", onOffRegulator);

    std::cout << "\n[Delegation Pattern]: ClimateManager delegates calculations to:" << std::endl;
    std::cout << "  - Temperature -> PID Regulator" << std::endl;
    std::cout << "  - Humidity -> On/Off Regulator with hysteresis" << std::endl;
    std::cout << "  - Soil moisture -> On/Off Regulator with hysteresis" << std::endl;

    // 6. Паттерн Proxy: оборачиваем в SafetyProxyManager
    auto safetyProxy = std::make_shared<SafetyProxyManager>(realClimateManager, io_manager);
    safetyProxy->setCriticalThresholds(5.0, 40.0, 90.0);

    std::cout << "\n[Proxy Pattern]: SafetyProxyManager wraps ClimateManager" << std::endl;
    std::cout << "  - Emergency thresholds: temp [5-40]C, humidity <90%" << std::endl;

    // 7. Расширенный ConfigManager с поддержкой таймеров
    auto configManager = std::make_shared<ExtendedConfigManager>();

    configManager->setTargetParameter("temperature", 23.0);
    configManager->setTargetParameter("air_humidity", 65.0);
    configManager->setTargetParameter("soil_moisture", 50.0);

    configManager->setSchedule("ventilation", 10, 50);
    configManager->setSchedule("lamp", 15, 70);

    std::cout << "\n[Configuration Pattern]: Schedules configured:" << std::endl;
    std::cout << "  - Ventilation: every 10s at 50%" << std::endl;
    std::cout << "  - Lamp: every 15s at 70%" << std::endl;

    // 8. Создаем движок - передаем shared_ptr на модель и ссылку на IOManager
    SimulationEngine engine(simulationModel, io_manager);
    engine.setClimateManager(safetyProxy);
    engine.setConfigManager(configManager);
    engine.setupSchedules(configManager.get());

    std::cout << "\n========================================" << std::endl;
    std::cout << "[INFO]: System fully configured" << std::endl;
    std::cout << "  - Target temperature: 23.0 C" << std::endl;
    std::cout << "  - Target humidity: 65.0 %" << std::endl;
    std::cout << "  - Target soil moisture: 50.0 %" << std::endl;
    std::cout << "\n[INFO]: Starting control system...\n" << std::endl;

    engine.start();

    return 0;
}