#include "SimulationEngine.h"
#include "ScheduleConfig.h"
#include <iostream>
#include <thread>
#include <chrono>

// Изменяем конструктор
SimulationEngine::SimulationEngine(std::shared_ptr<SimulationModel> model,
    std::shared_ptr<IOManager> io_manager)
    : m_model(model), m_io_manager(io_manager), m_is_running(false) {
}

SimulationEngine::~SimulationEngine() {
    stop();
}

void SimulationEngine::setClimateManager(std::shared_ptr<IClimateManager> manager) {
    m_climateManager = manager;
}

void SimulationEngine::setConfigManager(std::shared_ptr<IConfigManager> manager) {
    m_configManager = manager;
}

void SimulationEngine::setupSchedules(ExtendedConfigManager* configManager) {
    if (!configManager || !m_io_manager) return; 

    for (const auto& pair : configManager->getAllSchedules()) {
        const std::string& deviceType = pair.first;
        const ScheduleConfig& schedule = pair.second;

        if (!schedule.enabled) continue;

        auto timer = std::make_unique<TimerSchedule>();
        timer->start(schedule.interval, [this, deviceType, schedule]() {
            if (m_is_running) {
                applyScheduledCommand(deviceType, schedule.powerLevel);
            }
            });

        m_schedules[deviceType] = std::move(timer);
        std::cout << "[Engine]: Timer started for " << deviceType
            << " every " << schedule.interval.count() << "s" << std::endl;
    }
}

void SimulationEngine::applyScheduledCommand(const std::string& deviceType, int powerLevel) {
    if (!m_io_manager) return; 

    std::cout << "\n[Timer]: Executing scheduled command for " << deviceType
        << " at " << powerLevel << "%" << std::endl;

    auto deviceIds = m_io_manager->getDeviceIdsByType(deviceType);
    for (int deviceId : deviceIds) {
        m_io_manager->sendCommand(deviceId, powerLevel);

        if (deviceType == "ventilation" && m_model) {
            m_model->applyVentilationEffect(powerLevel);
        }
        else if (deviceType == "lamp" && m_model) {
            m_model->applyLampEffect(powerLevel);
        }
    }
}

void SimulationEngine::start() {
    if (!m_climateManager || !m_configManager || !m_model || !m_io_manager) {
        std::cout << "[ERROR]: Required components not set!" << std::endl;
        std::cout << "  ClimateManager: " << (m_climateManager ? "OK" : "MISSING") << std::endl;
        std::cout << "  ConfigManager: " << (m_configManager ? "OK" : "MISSING") << std::endl;
        std::cout << "  Model: " << (m_model ? "OK" : "MISSING") << std::endl;
        std::cout << "  IOManager: " << (m_io_manager ? "OK" : "MISSING") << std::endl;
        return;
    }

    m_is_running = true;

    auto targets = m_configManager->getAllTargets();
    m_climateManager->setTargetParameters(targets);

    std::cout << "\n[INFO]: SimulationEngine started with control loop" << std::endl;

    while (m_is_running) {
        auto sensorReadings = m_io_manager->readAllSensors();

        std::map<std::string, double> currentValues;
        for (const auto& pair : sensorReadings) {
            int id = pair.first;
            double value = pair.second;
            auto sensor = m_io_manager->getSensor(id);
            if (sensor) {
                currentValues[sensor->getType()] = value;
            }
        }

        std::cout << "\n=== Current readings ===" << std::endl;
        for (const auto& pair : currentValues) {
            const std::string& param = pair.first;
            double value = pair.second;
            std::cout << param << ": " << value;
            if (param == "temperature") std::cout << " C";
            if (param == "air_humidity") std::cout << " %";
            if (param == "soil_moisture") std::cout << " %";
            std::cout << std::endl;
        }

        auto commands = m_climateManager->calculateCommands(currentValues);
        applyCommands(commands);

        if (m_model) {
            m_model->update();
            m_model->printParameters();
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void SimulationEngine::applyCommands(const std::map<std::string, int>& commands) {
    if (!m_model || !m_io_manager) return;

    for (const auto& pair : commands) {
        const std::string& deviceType = pair.first;
        int power = pair.second;

        auto deviceIds = m_io_manager->getDeviceIdsByType(deviceType);
        for (int deviceId : deviceIds) {
            m_io_manager->sendCommand(deviceId, power);
        }

        if (deviceType == "heater") {
            m_model->applyHeaterEffect(power);
        }
        else if (deviceType == "conditioner") {
            m_model->applyConditionerEffect(power);
        }
        else if (deviceType == "air_humidifier") {
            m_model->applyHumidifierEffect(power);
        }
        else if (deviceType == "irrigation") {
            m_model->applyIrrigationEffect(power);
        }
    }
}

void SimulationEngine::stop() {
    m_is_running = false;
    for (auto& pair : m_schedules) {
        if (pair.second) {
            pair.second->stop();
        }
    }
    std::cout << "[INFO]: SimulationEngine stopped" << std::endl;
}