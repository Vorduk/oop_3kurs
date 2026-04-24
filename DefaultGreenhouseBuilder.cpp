#include "DefaultGreenhouseBuilder.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"
#include "TemperatureSensorsComposite.h"
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"
#include "DeviceLoggerDecorator.h"
#include "DevicePowerLimitDecorator.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"
#include "TemperatureEmergencyStrategy.h"
#include <iostream>
#include <stdexcept>

DefaultGreenhouseBuilder::DefaultGreenhouseBuilder() {
    
}

void DefaultGreenhouseBuilder::buildModel() {
    m_simulation_model = std::make_shared<SimulationModel>(22.0, 65.0, 45.0);
    std::cout << "[DefaultGreenhouseBuilder]: Simulation model built" << std::endl;
}

void DefaultGreenhouseBuilder::buildSensors() {
    if (!m_simulation_model) {
        throw std::runtime_error("Model must be built before sensors");
    }
    m_sensors.clear();

    // подкомпозит 1 (3 датчика температуры)
    auto sub_composite_1 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 3; ++i) {
        sub_composite_1->addSensor(std::make_shared<TemperatureSensor>(m_simulation_model));
    }

    // подкомпозит 2 (2 датчика температуры)
    auto sub_composite_2 = std::make_shared<TemperatureSensorsComposite>();
    for (int i = 0; i < 2; ++i) {
        sub_composite_2->addSensor(std::make_shared<TemperatureSensor>(m_simulation_model));
    }

    // корневой композит
    auto root_composite = std::make_shared<TemperatureSensorsComposite>();
    root_composite->addSensor(sub_composite_1);
    root_composite->addSensor(sub_composite_2);
    m_sensors.push_back(root_composite);

    // 2 датчика влажности воздуха
    for (int i = 0; i < 2; ++i) {
        m_sensors.push_back(std::make_shared<AirHumiditySensor>(m_simulation_model));
    }

    // 2 датчика влажности почвы
    for (int i = 0; i < 2; ++i) {
        m_sensors.push_back(std::make_shared<SoilMoistureSensor>(m_simulation_model));
    }

    std::cout << "[DefaultGreenhouseBuilder]: Sensors built (temp composite + air/soil)" << std::endl;
}

void DefaultGreenhouseBuilder::buildDevices() {
    m_devices.clear();

    // нагреватель с декораторами
    auto heater_core = std::make_shared<Heater>();
    auto limited_heater = std::make_shared<DevicePowerLimitDecorator>(heater_core, 10);
    auto logged_limited_heater = std::make_shared<DeviceLoggerDecorator>(limited_heater);
    m_devices.push_back(logged_limited_heater);

    // кондиционер с декораторами
    auto conditioner_core = std::make_shared<Conditioner>();
    auto logged_conditioner = std::make_shared<DeviceLoggerDecorator>(conditioner_core);
    auto limited_logged_conditioner = std::make_shared<DevicePowerLimitDecorator>(logged_conditioner, 50);
    m_devices.push_back(limited_logged_conditioner);

    // остальные устройства
    m_devices.push_back(std::make_shared<AirHumidifier>());
    m_devices.push_back(std::make_shared<Irrigation>());
    m_devices.push_back(std::make_shared<Ventilation>());
    m_devices.push_back(std::make_shared<Lamp>());

    std::cout << "[DefaultGreenhouseBuilder]: Devices built with decorators" << std::endl;
}

void DefaultGreenhouseBuilder::buildIOManager() {
    m_io_manager = std::make_shared<IOManager>();

    for (const auto& device : m_devices) {
        m_io_manager->addDevice(device);
    }

    for (const auto& sensor : m_sensors) {
        m_io_manager->addSensor(sensor);
    }

    std::cout << "[DefaultGreenhouseBuilder]: IO manager built and populated" << std::endl;
}

void DefaultGreenhouseBuilder::buildClimateManager() {
    m_real_climate_manager = std::make_shared<ClimateManager>();

    auto pid_regulator = std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
    auto onoff_regulator = std::make_shared<OnOffRegulator>(2.0);

    m_real_climate_manager->setRegulator("temperature", pid_regulator);
    m_real_climate_manager->setRegulator("air_humidity", onoff_regulator);
    m_real_climate_manager->setRegulator("soil_moisture", onoff_regulator);

    auto emergency_strategy = std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);
    m_safety_proxy = std::make_shared<SafetyProxyManager>(
        m_real_climate_manager,
        m_io_manager,
        emergency_strategy
    );

    std::cout << "[DefaultGreenhouseBuilder]: Climate manager with safety proxy built" << std::endl;
}

void DefaultGreenhouseBuilder::buildConfig() {
    m_config_manager = std::make_shared<ExtendedConfigManager>();

    m_config_manager->setTargetParameter("temperature", 23.0);
    m_config_manager->setTargetParameter("air_humidity", 65.0);
    m_config_manager->setTargetParameter("soil_moisture", 50.0);

    m_config_manager->setSchedule("ventilation", 10, 50);
    m_config_manager->setSchedule("lamp", 15, 70);

    std::cout << "[DefaultGreenhouseBuilder]: Configuration built" << std::endl;
}

void DefaultGreenhouseBuilder::buildEngine() {
    if (!m_io_manager || !m_simulation_model) {
        throw std::runtime_error("IOManager and model must be built before engine");
    }

    m_engine = std::make_shared<SimulationEngine>(m_simulation_model, m_io_manager);

    if (m_safety_proxy) {
        m_engine->setClimateManager(m_safety_proxy);
    }
    if (m_config_manager) {
        m_engine->setConfigManager(m_config_manager);
        m_engine->setupSchedules(m_config_manager.get());
    }

    std::cout << "[DefaultGreenhouseBuilder]: Simulation engine assembled" << std::endl;
}

std::shared_ptr<SimulationEngine> DefaultGreenhouseBuilder::getResult() {
    return m_engine;
}