#include "GreenhouseSystemFacade.h"

// Подсистемы
#include "SimulationModel.h"
#include "IOManager.h"
#include "ClimateManager.h"
#include "SafetyProxyManager.h"
#include "TemperatureEmergencyStrategy.h"
#include "ScheduleConfig.h"
#include "SimulationEngine.h"
#include "PidRegulator.h"
#include "OnOffRegulator.h"

// Датчики
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
#include "SoilMoistureSensor.h"

// Устройства
#include "Heater.h"
#include "Conditioner.h"
#include "AirHumidifier.h"
#include "Irrigation.h"
#include "Ventilation.h"
#include "Lamp.h"

#include <iostream>

GreenhouseSystemFacade::GreenhouseSystemFacade()
    : m_initialized(false) {
    std::cout << "[GreenhouseSystemFacade] Creating facade..." << std::endl;
    initialize();
    std::cout << "[GreenhouseSystemFacade] Facade initialized successfully." << std::endl;
}

GreenhouseSystemFacade::~GreenhouseSystemFacade() {
    std::cout << "[GreenhouseSystemFacade] Destroying facade..." << std::endl;
    stop();
}

void GreenhouseSystemFacade::initialize() {
    // Создание модели симуляции с начальными значениями (по умолчанию)
    double initialTemp = 22.0;
    double initialAirHum = 65.0;
    double initialSoil = 45.0;
    m_model = std::make_shared<SimulationModel>(initialTemp, initialAirHum, initialSoil);
    std::cout << "[GreenhouseSystemFacade] SimulationModel created." << std::endl;

    // Создание IOManager
    m_io_manager = std::make_shared<IOManager>();
    std::cout << "[GreenhouseSystemFacade] IOManager created." << std::endl;

    // Создание датчиков и устройств
    createSensors();
    createDevices();

    // Настройка менеджера климата (реального) с регуляторами
    std::shared_ptr<ClimateManager> realClimateManager = std::make_shared<ClimateManager>();
    std::shared_ptr<PidRegulator> pidRegulator = std::make_shared<PidRegulator>(2.0, 0.5, 1.0);
    std::shared_ptr<OnOffRegulator> onOffRegulator = std::make_shared<OnOffRegulator>(2.0);

    realClimateManager->setRegulator("temperature", pidRegulator);
    realClimateManager->setRegulator("air_humidity", onOffRegulator);
    realClimateManager->setRegulator("soil_moisture", onOffRegulator);
    std::cout << "[GreenhouseSystemFacade] ClimateManager with regulators configured." << std::endl;

    // Создание прокси безопасности
    std::shared_ptr<IEmergencyStrategy> emergencyStrategy =
        std::make_shared<TemperatureEmergencyStrategy>(5.0, 40.0, 90.0);
    m_climate_manager = std::make_shared<SafetyProxyManager>(
        realClimateManager,
        m_io_manager,
        emergencyStrategy
    );
    std::cout << "[GreenhouseSystemFacade] SafetyProxyManager created." << std::endl;

    // Создание менеджера конфигурации
    m_config_manager = std::make_shared<ExtendedConfigManager>();
    // Установка целевых параметров по умолчанию
    m_config_manager->setTargetParameter("temperature", 22.0);
    m_config_manager->setTargetParameter("air_humidity", 65.0);
    m_config_manager->setTargetParameter("soil_moisture", 45.0);
    std::cout << "[GreenhouseSystemFacade] ExtendedConfigManager created with default targets." << std::endl;

    // Создание движка симуляции
    m_engine = std::make_unique<SimulationEngine>(m_model, m_io_manager);
    m_engine->setClimateManager(m_climate_manager);
    m_engine->setConfigManager(m_config_manager);
    std::cout << "[GreenhouseSystemFacade] SimulationEngine created." << std::endl;

    m_initialized = true;
}

void GreenhouseSystemFacade::createSensors() {
    std::cout << "[GreenhouseSystemFacade] Creating sensors..." << std::endl;

    // Создание датчиков
    const int TEMP_SENSORS_COUNT = 5;
    const int AIR_HUM_SENSORS_COUNT = 2;
    const int SOIL_SENSORS_COUNT = 2;

    for (int i = 0; i < TEMP_SENSORS_COUNT; ++i) {
        std::shared_ptr<TemperatureSensor> sensor = std::make_shared<TemperatureSensor>(m_model);
        m_io_manager->addSensor(sensor);
        m_sensors.push_back(sensor);
    }

    for (int i = 0; i < AIR_HUM_SENSORS_COUNT; ++i) {
        std::shared_ptr<AirHumiditySensor> sensor = std::make_shared<AirHumiditySensor>(m_model);
        m_io_manager->addSensor(sensor);
        m_sensors.push_back(sensor);
    }

    for (int i = 0; i < SOIL_SENSORS_COUNT; ++i) {
        std::shared_ptr<SoilMoistureSensor> sensor = std::make_shared<SoilMoistureSensor>(m_model);
        m_io_manager->addSensor(sensor);
        m_sensors.push_back(sensor);
    }

    std::cout << "[GreenhouseSystemFacade] Created " << TEMP_SENSORS_COUNT << " temperature sensors, "
        << AIR_HUM_SENSORS_COUNT << " air humidity sensors, "
        << SOIL_SENSORS_COUNT << " soil moisture sensors." << std::endl;
}

void GreenhouseSystemFacade::createDevices() {
    std::cout << "[GreenhouseSystemFacade] Creating devices (without decorators)..." << std::endl;

    // Нагреватель
    std::shared_ptr<BaseDevice> heater = std::make_shared<Heater>();
    m_io_manager->addDevice(heater);
    m_devices.push_back(heater);
    std::cout << "[GreenhouseSystemFacade]   - Heater (ID: " << heater->getId() << ")" << std::endl;

    // Кондиционер
    std::shared_ptr<BaseDevice> conditioner = std::make_shared<Conditioner>();
    m_io_manager->addDevice(conditioner);
    m_devices.push_back(conditioner);
    std::cout << "[GreenhouseSystemFacade]   - Conditioner (ID: " << conditioner->getId() << ")" << std::endl;

    // Увлажнитель воздуха
    std::shared_ptr<BaseDevice> humidifier = std::make_shared<AirHumidifier>();
    m_io_manager->addDevice(humidifier);
    m_devices.push_back(humidifier);
    std::cout << "[GreenhouseSystemFacade]   - AirHumidifier (ID: " << humidifier->getId() << ")" << std::endl;

    // Система полива
    std::shared_ptr<BaseDevice> irrigation = std::make_shared<Irrigation>();
    m_io_manager->addDevice(irrigation);
    m_devices.push_back(irrigation);
    std::cout << "[GreenhouseSystemFacade]   - Irrigation (ID: " << irrigation->getId() << ")" << std::endl;

    // Вентиляция
    std::shared_ptr<BaseDevice> ventilation = std::make_shared<Ventilation>();
    m_io_manager->addDevice(ventilation);
    m_devices.push_back(ventilation);
    std::cout << "[GreenhouseSystemFacade]   - Ventilation (ID: " << ventilation->getId() << ")" << std::endl;

    // Лампы
    std::shared_ptr<BaseDevice> lamp = std::make_shared<Lamp>();
    m_io_manager->addDevice(lamp);
    m_devices.push_back(lamp);
    std::cout << "[GreenhouseSystemFacade]   - Lamp (ID: " << lamp->getId() << ")" << std::endl;

    std::cout << "[GreenhouseSystemFacade] All devices created and registered." << std::endl;
}

void GreenhouseSystemFacade::setTargetParameter(const std::string& parameter, double value) {
    if (m_config_manager) {
        m_config_manager->setTargetParameter(parameter, value);
        std::cout << "[GreenhouseSystemFacade] Target parameter '" << parameter
            << "' set to " << value << std::endl;
    }
}

void GreenhouseSystemFacade::setSchedule(const std::string& deviceType,
    int intervalSeconds, int powerLevel) {
    if (m_config_manager) {
        m_config_manager->setSchedule(deviceType, intervalSeconds, powerLevel);
        std::cout << "[GreenhouseSystemFacade] Schedule for '" << deviceType
            << "' set: every " << intervalSeconds << "s at " << powerLevel << "%" << std::endl;
    }
    // Передача расписаний движку
    if (m_engine && m_config_manager) {
        m_engine->setupSchedules(m_config_manager.get());
    }
}

void GreenhouseSystemFacade::start() {
    if (!m_initialized) {
        std::cerr << "[GreenhouseSystemFacade] Error: Facade not initialized!" << std::endl;
        return;
    }

    std::cout << "[GreenhouseSystemFacade] Starting simulation engine..." << std::endl;
    m_engine->start();
}

void GreenhouseSystemFacade::stop() {
    if (m_engine) {
        std::cout << "[GreenhouseSystemFacade] Stopping simulation engine..." << std::endl;
        m_engine->stop();
    }
}