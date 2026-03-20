#include "SimulationEngine.h"
#include <iostream>
#include <thread>
#include <chrono>

SimulationEngine::SimulationEngine(SimulationModel& model, IOManager& io_manager)
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

void SimulationEngine::start() {
    if (!m_climateManager || !m_configManager) {
        std::cout << "[ERROR]: ClimateManager or ConfigManager not set!" << std::endl;
        return;
    }

    m_is_running = true;

    // Устанавка целевых параметров
    auto targets = m_configManager->getAllTargets();
    m_climateManager->setTargetParameters(targets);

    std::cout << "\n[INFO]: SimulationEngine started with control loop" << std::endl;

    // Главный цикл
    while (m_is_running) {
        m_model.update();
        m_model.printParameters();

        std::map<int, double> sensorReadings = m_io_manager.readAllSensors();

        // Показания в map<parameter, value>
        std::map<std::string, double> currentValues;
        for (const auto& [id, value] : sensorReadings) {
            ISensor* sensor = m_io_manager.getSensor(id);
            if (sensor) {
                currentValues[sensor->getType()] = value;
            }
        }

        // Вывод текущих показаний
        std::cout << "\n=== Current readings (by parameter) ===" << std::endl;
        for (const auto& [param, value] : currentValues) {
            std::cout << param << ": " << value;
            if (param == "temperature") std::cout << " C";
            if (param == "air_humidity") std::cout << " %";
            if (param == "soil_moisture") std::cout << " %";
            std::cout << std::endl;
        }

        // Рассчёт команд
        auto commands = m_climateManager->calculateCommands(currentValues);

        // Применение команд к устройствам
        applyCommands(commands);

        // Задержка перед следующим циклом
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void SimulationEngine::applyCommands(const std::map<std::string, int>& commands) {
    for (const auto& [deviceType, power] : commands) {
        // Поиск всех устройств данного типа и отправка команды
        // Пока упрощенно: предполагается, что есть по одному устройству каждого типа
        // В реальном коде нужно получать ID устройства по типу

        std::cout << "[Engine]: Sending command to " << deviceType
            << " -> power=" << power << "%" << std::endl;

        // TODO: Здесь нужно найти ID устройства по типу
    }
}

void SimulationEngine::stop() {
    m_is_running = false;
    std::cout << "[INFO]: SimulationEngine stopped" << std::endl;
}