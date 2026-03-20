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

    // Лабораторная 4 - Проектирование автоматизированной системы управления микроклиматом в автоматизированной теплице.
    // Паттерны: Delegation, Proxy, Configuration.

    // Шапка
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System - Lab 4" << std::endl;
    std::cout << "Patterns: Delegation, Proxy, Configuration" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Создание модели симуляции с начальными параметрами (температура, влажность воздуха, влажность почвы)
    // Подразумевается, что в модели одна грядка в теплице, поэтому влажность почвы одна.
    SimulationModel simulationModel(22.0, 65.0, 45.0);

    // Создание датчиков
    TemperatureSensor tempSensor(&simulationModel);
    AirHumiditySensor humiditySensor(&simulationModel);
    SoilMoistureSensor soilSensor(&simulationModel);

    // Создание устройств
    Heater heater;
    Conditioner conditioner;
    AirHumidifier humidifier;
    Irrigation irrigation;
    Ventilation ventilation;
    Lamp lamp;

    // Настройка IOManager
    IOManager io_manager;
    io_manager.addSensor(&tempSensor);
    io_manager.addSensor(&humiditySensor);
    io_manager.addSensor(&soilSensor);
    io_manager.addDevice(&heater);
    io_manager.addDevice(&conditioner);
    io_manager.addDevice(&humidifier);
    io_manager.addDevice(&irrigation);
    io_manager.addDevice(&ventilation);
    io_manager.addDevice(&lamp);

    // Реальный ClimateManager с делегированием
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
    auto safetyProxy = std::make_shared<SafetyProxyManager>(realClimateManager, &io_manager);
    safetyProxy->setCriticalThresholds(5.0, 40.0, 90.0);

    std::cout << "\n[Proxy Pattern]: SafetyProxyManager wraps ClimateManager" << std::endl;
    std::cout << "  - Emergency thresholds: temp [5-40]C, humidity <90%" << std::endl;

    // 7. Расширенный ConfigManager с поддержкой таймеров
    auto configManager = std::make_shared<ExtendedConfigManager>();

    // Устанавливаем целевые параметры
    configManager->setTargetParameter("temperature", 23.0);
    configManager->setTargetParameter("air_humidity", 65.0);
    configManager->setTargetParameter("soil_moisture", 50.0);

    // Паттерн Configuration: настраиваем таймеры для устройств
    configManager->setSchedule("ventilation", 10, 50);   // Вентиляция каждые 10 сек на 50%
    configManager->setSchedule("lamp", 15, 70);          // Лампы каждые 15 сек на 70%

    std::cout << "\n[Configuration Pattern]: Schedules configured:" << std::endl;
    std::cout << "  - Ventilation: every 10s at 50%" << std::endl;
    std::cout << "  - Lamp: every 15s at 70%" << std::endl;

    // 8. Создаем движок и настраиваем
    SimulationEngine engine(simulationModel, io_manager);
    engine.setClimateManager(safetyProxy);      // Используем Proxy вместо реального менеджера
    engine.setConfigManager(configManager);
    engine.setupSchedules(configManager.get()); // Запускаем таймеры

    std::cout << "\n========================================" << std::endl;
    std::cout << "[INFO]: System fully configured" << std::endl;
    std::cout << "  - Target temperature: 23.0 C" << std::endl;
    std::cout << "  - Target humidity: 65.0 %" << std::endl;
    std::cout << "  - Target soil moisture: 50.0 %" << std::endl;
    std::cout << "\n[INFO]: Starting control system...\n" << std::endl;

    engine.start();

    return 0;
}