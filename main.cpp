#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>

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
#include "OldHeaterAdapter.h"
#include "DeviceLoggerDecorator.h"
#include "DevicePowerLimitDecorator.h"
#include "TemperatureSensorsComposite.h"
#include "GreenhouseBoss.h"
#include "HistoryManager.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Greenhouse Control System with Memento" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Модель – источник данных для датчиков
    std::shared_ptr<SimulationModel> simulation_model = std::make_shared<SimulationModel>(22.0, 65.0, 45.0);

    // Датчики
    std::vector<std::shared_ptr<BaseSensor>> all_sensors;
    std::cout << "\nCreating sensors..." << std::endl;

    std::shared_ptr<TemperatureSensorsComposite> subComposite1 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 3; ++i) {
        std::shared_ptr<TemperatureSensor> tempSensor = std::make_shared<TemperatureSensor>(simulation_model);
        subComposite1->addSensor(tempSensor);
    }
    std::shared_ptr<TemperatureSensorsComposite> subComposite2 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<TemperatureSensor> tempSensor = std::make_shared<TemperatureSensor>(simulation_model);
        subComposite2->addSensor(tempSensor);
    }
    std::shared_ptr<TemperatureSensorsComposite> rootComposite = std::make_shared<TemperatureSensorsComposite>();
    rootComposite->addSensor(subComposite1);
    rootComposite->addSensor(subComposite2);
    all_sensors.push_back(rootComposite);

    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<BaseSensor> sensor = std::make_shared<AirHumiditySensor>(simulation_model);
        all_sensors.push_back(sensor);
        std::cout << "Created sensor: Type='" << sensor->getType() << "', ID=" << sensor->getId() << std::endl;
    }
    for (int i = 0; i < 2; ++i) {
        std::shared_ptr<BaseSensor> sensor = std::make_shared<SoilMoistureSensor>(simulation_model);
        all_sensors.push_back(sensor);
        std::cout << "Created sensor: Type='" << sensor->getType() << "', ID=" << sensor->getId() << std::endl;
    }
    std::cout << "\nTotal sensors created: " << all_sensors.size() << std::endl;

    // Устройства
    std::cout << "\nCreating devices..." << std::endl;
    std::vector<std::shared_ptr<BaseDevice>> allDevices;
    std::shared_ptr<BaseDevice> humidifier = std::make_shared<AirHumidifier>();
    allDevices.push_back(humidifier);
    std::shared_ptr<BaseDevice> irrigation = std::make_shared<Irrigation>();
    allDevices.push_back(irrigation);
    std::shared_ptr<BaseDevice> ventilation = std::make_shared<Ventilation>();
    allDevices.push_back(ventilation);
    std::shared_ptr<BaseDevice> lamp = std::make_shared<Lamp>();
    allDevices.push_back(lamp);

    // IOManager
    std::shared_ptr<IOManager> io_manager = std::make_shared<IOManager>();
    std::shared_ptr<BaseDevice> heater = std::make_shared<Heater>();
    std::shared_ptr<IDevice> limited_heater = std::make_shared<DevicePowerLimitDecorator>(heater, 10);
    std::shared_ptr<IDevice> logged_limited_heater = std::make_shared<DeviceLoggerDecorator>(limited_heater);
    io_manager->addDevice(logged_limited_heater);
    std::shared_ptr<IDevice> conditioner = std::make_shared<Conditioner>();
    std::shared_ptr<IDevice> logged_conditioner = std::make_shared<DeviceLoggerDecorator>(conditioner);
    std::shared_ptr<IDevice> limited_logged_conditioner = std::make_shared<DevicePowerLimitDecorator>(logged_conditioner, 50);
    io_manager->addDevice(limited_logged_conditioner);

    for (std::shared_ptr<BaseSensor>& sensor : all_sensors) {
        io_manager->addSensor(sensor);
    }
    for (std::shared_ptr<BaseDevice>& device : allDevices) {
        io_manager->addDevice(device);
    }

    // Климат менеджер
    std::shared_ptr<ClimateManager> realClimateManager = std::make_shared<ClimateManager>();
    std::shared_ptr<PidRegulator> pidRegulator = std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
    std::shared_ptr<OnOffRegulator> onOffRegulator = std::make_shared<OnOffRegulator>(2.0);
    realClimateManager->setRegulator("temperature", pidRegulator);
    realClimateManager->setRegulator("air_humidity", onOffRegulator);
    realClimateManager->setRegulator("soil_moisture", onOffRegulator);

    std::shared_ptr<ExtendedConfigManager> configManager = std::make_shared<ExtendedConfigManager>();
    double targetTemp = 23.0, targetHum = 65.0, targetSoil = 50.0;
    configManager->setTargetParameter("temperature", targetTemp);
    configManager->setTargetParameter("air_humidity", targetHum);
    configManager->setTargetParameter("soil_moisture", targetSoil);
    realClimateManager->setTargetParameters(configManager->getAllTargets());

    // GreenhouseBoss и HistoryManager для паттерна Memento
    std::shared_ptr<GreenhouseBoss> boss = std::make_shared<GreenhouseBoss>(simulation_model, realClimateManager);
    HistoryManager history;

    // Демонстрация Memento
    std::cout << "\nMemento demonstration" << std::endl;
    std::cout << "Initial state:" << std::endl;
    std::cout << "Temperature: " << simulation_model->getTemperature() << " C" << std::endl;
    std::cout << "Air humidity: " << simulation_model->getAirHumidity() << " %" << std::endl;
    std::cout << "Soil moisture: " << simulation_model->getSoilMoisture() << " %" << std::endl;

    // Сохранение первого снимка
    history.push(boss->createMemento());

    // Новые целевые параметры и несколько обновлений модели
    std::map<std::string, double> newTargets;
    newTargets["temperature"] = 28.0;
    newTargets["air_humidity"] = 70.0;
    newTargets["soil_moisture"] = 60.0;
    realClimateManager->setTargetParameters(newTargets);

    // Имитация работы системы – несколько вызовов update
    for (int i = 0; i < 3; ++i) {
        simulation_model->applyHeaterEffect(30);
        simulation_model->applyHumidifierEffect(20);
        simulation_model->applyIrrigationEffect(10);
        simulation_model->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "\nState after changes and 3 updates:" << std::endl;
    std::cout << "Temperature: " << simulation_model->getTemperature() << " C" << std::endl;
    std::cout << "Air humidity: " << simulation_model->getAirHumidity() << " %" << std::endl;
    std::cout << "Soil moisture: " << simulation_model->getSoilMoisture() << " %" << std::endl;

    // Сохранение второго снимка
    history.push(boss->createMemento());

    // Ещё изменения
    newTargets["temperature"] = 18.0;
    realClimateManager->setTargetParameters(newTargets);
    simulation_model->applyConditionerEffect(40);
    simulation_model->update();
    std::cout << "\nState after further changes:" << std::endl;
    std::cout << "Temperature: " << simulation_model->getTemperature() << " C" << std::endl;
    std::cout << "Air humidity: " << simulation_model->getAirHumidity() << " %" << std::endl;
    std::cout << "Soil moisture: " << simulation_model->getSoilMoisture() << " %" << std::endl;

    // Восстанавление первого снимка
    SystemMemento restoredMemento;
    if (history.pop(restoredMemento)) {
        boss->restoreFromMemento(restoredMemento);
    }
    std::cout << "\nAfter restoring first snapshot:" << std::endl;
    std::cout << "Temperature: " << simulation_model->getTemperature() << " C" << std::endl;
    std::cout << "Air humidity: " << simulation_model->getAirHumidity() << " %" << std::endl;
    std::cout << "Soil moisture: " << simulation_model->getSoilMoisture() << " %" << std::endl;

    // Восстанавление второго снимка
    if (history.pop(restoredMemento)) {
        boss->restoreFromMemento(restoredMemento);
    }
    std::cout << "\nAfter restoring second snapshot:" << std::endl;
    std::cout << "Temperature: " << simulation_model->getTemperature() << " C" << std::endl;
    std::cout << "Air humidity: " << simulation_model->getAirHumidity() << " %" << std::endl;
    std::cout << "Soil moisture: " << simulation_model->getSoilMoisture() << " %" << std::endl;

    // Запуск основного цикла
    /*std::cout << "\n========================================" << std::endl;
    std::cout << "Starting main control loop..." << std::endl;
    SimulationEngine engine(simulation_model, io_manager);
    engine.setClimateManager(realClimateManager);
    engine.setConfigManager(configManager);
    engine.setupSchedules(configManager.get());
    engine.start();*/

    return 0;
}